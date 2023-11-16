// SPDX-License-Identifier: MIT

#include "elos/connectionmanager/connectionmanager.h"

#include <arpa/inet.h>
#include <errno.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <stdlib.h>
#include <sys/eventfd.h>

#include "connectionmanager_private.h"
#include "elos/config/config.h"
#include "elos/connectionmanager/clientauthorization.h"
#include "elos/connectionmanager/clientauthorizedprocesses.h"
#include "elos/connectionmanager/clientblacklist.h"
#include "elos/connectionmanager/clientconnection.h"

static safuResultE_t _initializeSharedData(elosConnectionManager_t *connectionManager,
                                           elosConnectionManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionSharedData_t *sharedData = &connectionManager->sharedData;
    int retVal;

    retVal = sem_init(&sharedData->connectionSemaphore, 0, ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT);
    if (retVal != 0) {
        safuLogErrErrnoValue("sem_init failed!", retVal);
    } else {
        sharedData->logAggregator = param->logAggregator;
        sharedData->eventProcessor = param->eventProcessor;
        sharedData->eventDispatcher = param->eventDispatcher;
        sharedData->config = param->config;
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _initializeListener(elosConnectionManager_t *connectionManager, samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr_in *addr = &connectionManager->addr;
    char const *interface = elosConfigGetElosdInterface(config);
    int const port = elosConfigGetElosdPort(config);
    int retVal;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    retVal = inet_pton(AF_INET, interface, &addr->sin_addr);
    if (retVal != 1) {
        safuLogErrErrnoValue("inet_pton failed", retVal);
    } else {
        connectionManager->fd = socket(addr->sin_family, SOCK_STREAM, 0);
        if (connectionManager->fd == -1) {
            safuLogErrErrnoValue("socket failed", connectionManager->fd);
        } else {
            retVal = setsockopt(connectionManager->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
            if (retVal != 0) {
                safuLogErrErrnoValue("setsocketopt SO_REUSEADDR failed", retVal);
            } else {
                retVal = bind(connectionManager->fd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
                if (retVal != 0) {
                    safuLogErrErrnoValue("bind failed", retVal);
                } else {
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

static safuResultE_t _initializeConnections(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (int i = 0; i < ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT; i += 1) {
        elosClientConnection_t *connection = &connectionManager->connection[i];
        elosClientConnectionParam_t param = {.sharedData = &connectionManager->sharedData};

        result = elosClientConnectionInitialize(connection, &param);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Iniitialization of connection data structure [%d] failed", i);
            break;
        }
    }

    return result;
}

static safuResultE_t _initializeAuthorization(elosConnectionManager_t *connectionManager,
                                              samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosClientAuthorizationInitialize(&connectionManager->clientAuth);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosClientAuthorizationInitialize failed");
    }

    result = elosAuthorizedProcessInitialize(&connectionManager->clientAuth.authorizedProcessFilters, config);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("authorized process filter initialization failed");
    } else {
        safuLogDebug("authorized process filter initialization succeeded");
    }

    return result;
}

safuResultE_t elosConnectionManagerInitialize(elosConnectionManager_t *connectionManager,
                                              elosConnectionManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((connectionManager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if ((param->config == NULL) || (param->eventDispatcher == NULL) || (param->eventProcessor == NULL) ||
               (param->logAggregator == NULL)) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == true) {
        safuLogErr("The given ConnectionManager is already initialized");
    } else {
        memset(connectionManager, 0, sizeof(elosConnectionManager_t));

        result = _initializeSharedData(connectionManager, param);
        if (result == SAFU_RESULT_OK) {
            result = _initializeListener(connectionManager, param->config);
            if (result == SAFU_RESULT_OK) {
                result = _initializeConnections(connectionManager);
                if (result == SAFU_RESULT_OK) {
                    connectionManager->syncFd = eventfd(0, 0);
                    if (connectionManager->syncFd == -1) {
                        safuLogErrErrnoValue("Creating eventfd failed", connectionManager->syncFd);
                    } else {
                        _initializeAuthorization(connectionManager, param->config);

                        atomic_store(&connectionManager->flags, SAFU_FLAG_INITIALIZED_BIT);
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosConnectionManagerDeleteMembers(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    if (connectionManager != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == true) {
            safuResultE_t iterResult;

            iterResult = elosConnectionManagerStop(connectionManager);
            if (iterResult != SAFU_RESULT_OK) {
                safuLogWarn("Stopping ConnectionManager failed (possible memory leak)");
                result = SAFU_RESULT_FAILED;
            }

            for (int i = 0; i < ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT; i += 1) {
                elosClientConnection_t *connection = &connectionManager->connection[i];

                iterResult = elosClientConnectionDeleteMembers(connection);
                if (iterResult != SAFU_RESULT_OK) {
                    result = SAFU_RESULT_FAILED;
                }
            }

            retVal = sem_destroy(&connectionManager->sharedData.connectionSemaphore);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Destroying semaphore failed (possible memory leak)", retVal);
                result = SAFU_RESULT_FAILED;
            }

            elosClientAuthorizationDelete(&connectionManager->clientAuth);

            if (connectionManager->fd != -1) {
                retVal = close(connectionManager->fd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing listenFd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (connectionManager->syncFd != -1) {
                retVal = close(connectionManager->syncFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}
