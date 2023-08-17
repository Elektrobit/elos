// SPDX-License-Identifier: MIT

#include "elos/clientmanager/clientmanager.h"

#include <arpa/inet.h>
#include <errno.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <sys/eventfd.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientauthorizedprocesses.h"
#include "elos/clientmanager/clientblacklist.h"
#include "elos/clientmanager/clientconnection.h"
#include "elos/config/config.h"

static safuResultE_t _initializeSharedData(elosClientManager_t *clientmanager, elosClientManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionSharedData_t *sharedData = &clientmanager->sharedData;
    int retVal;

    retVal = sem_init(&sharedData->connectionSemaphore, 0, ELOS_CLIENTMANAGER_CONNECTION_LIMIT);
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

static safuResultE_t _initializeListener(elosClientManager_t *clientmanager, samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr_in *addr = &clientmanager->addr;
    char const *interface = elosConfigGetElosdInterface(config);
    int const port = elosConfigGetElosdPort(config);
    int retVal;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    retVal = inet_pton(AF_INET, interface, &addr->sin_addr);
    if (retVal != 1) {
        safuLogErrErrnoValue("inet_pton failed", retVal);
    } else {
        clientmanager->fd = socket(addr->sin_family, SOCK_STREAM, 0);
        if (clientmanager->fd == -1) {
            safuLogErrErrnoValue("socket failed", clientmanager->fd);
        } else {
            retVal = setsockopt(clientmanager->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
            if (retVal != 0) {
                safuLogErrErrnoValue("setsocketopt SO_REUSEADDR failed", retVal);
            } else {
                retVal = bind(clientmanager->fd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
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

static safuResultE_t _initializeConnections(elosClientManager_t *clientmanager) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (int i = 0; i < ELOS_CLIENTMANAGER_CONNECTION_LIMIT; i += 1) {
        elosClientConnection_t *connection = &clientmanager->connection[i];
        elosClientConnectionParam_t param = {.sharedData = &clientmanager->sharedData};

        result = elosClientConnectionInitialize(connection, &param);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Iniitialization of connection data structure [%d] failed", i);
            break;
        }
    }

    return result;
}

static safuResultE_t _initializeAuthorization(elosClientManager_t *clientmanager, samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosClientAuthorizationInitialize(&clientmanager->clientAuth);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosClientAuthorizationInitialize failed");
    }

    result = elosAuthorizedProcessInitialize(&clientmanager->clientAuth.authorizedProcessFilters, config);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("authorized process filter initialization failed");
    } else {
        safuLogDebug("authorized process filter initialization succeeded");
    }

    return result;
}

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientmanager, elosClientManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientmanager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if ((param->config == NULL) || (param->eventDispatcher == NULL) || (param->eventProcessor == NULL) ||
               (param->logAggregator == NULL)) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientmanager->flags) == true) {
        safuLogErr("The given ClientManager is already initialized");
    } else {
        memset(clientmanager, 0, sizeof(elosClientManager_t));

        result = _initializeSharedData(clientmanager, param);
        if (result == SAFU_RESULT_OK) {
            result = _initializeListener(clientmanager, param->config);
            if (result == SAFU_RESULT_OK) {
                result = _initializeConnections(clientmanager);
                if (result == SAFU_RESULT_OK) {
                    clientmanager->syncFd = eventfd(0, 0);
                    if (clientmanager->syncFd == -1) {
                        safuLogErrErrnoValue("Creating eventfd failed", clientmanager->syncFd);
                    } else {
                        _initializeAuthorization(clientmanager, param->config);

                        atomic_store(&clientmanager->flags, SAFU_FLAG_INITIALIZED_BIT);
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    if (clientManager != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientManager->flags) == true) {
            safuResultE_t iterResult;

            iterResult = elosClientManagerStop(clientManager);
            if (iterResult != SAFU_RESULT_OK) {
                safuLogWarn("Stopping ClientManager failed (possible memory leak)");
                result = SAFU_RESULT_FAILED;
            }

            for (int i = 0; i < ELOS_CLIENTMANAGER_CONNECTION_LIMIT; i += 1) {
                elosClientConnection_t *connection = &clientManager->connection[i];

                iterResult = elosClientConnectionDeleteMembers(connection);
                if (iterResult != SAFU_RESULT_OK) {
                    result = SAFU_RESULT_FAILED;
                }
            }

            retVal = sem_destroy(&clientManager->sharedData.connectionSemaphore);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Destroying semaphore failed (possible memory leak)", retVal);
                result = SAFU_RESULT_FAILED;
            }

            elosClientAuthorizationDelete(&clientManager->clientAuth);

            if (clientManager->fd != -1) {
                retVal = close(clientManager->fd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing listenFd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (clientManager->syncFd != -1) {
                retVal = close(clientManager->syncFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}
