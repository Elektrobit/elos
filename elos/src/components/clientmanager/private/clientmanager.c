// SPDX-License-Identifier: MIT

#include "elos/clientmanager/clientmanager.h"

#include <arpa/inet.h>
#include <errno.h>
#include <safu/log.h>
#include <samconf/samconf.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientauthorizedprocesses.h"
#include "elos/clientmanager/clientblacklist.h"
#include "elos/config/config.h"

static safuResultE_t _initializeSharedData(elosClientManager_t *clientmanager, elosClientManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientManagerSharedData_t *sharedData = &clientmanager->sharedData;
    int retVal;

    retVal = sem_init(&sharedData->connectionSemaphore, 0, CLIENT_MANAGER_MAX_CONNECTIONS);
    if (retVal != 0) {
        safuLogErrErrnoValue("sem_init failed!", retVal);
    } else {
        sharedData->logAggregator = param->logAggregator;
        sharedData->eventProcessor = param->eventProcessor;
        sharedData->eventDispatcher = param->eventDispatcher;
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
                    // TODO: Move into Start()
                    retVal = listen(clientmanager->fd, CLIENT_MANAGER_LISTEN_QUEUE_LENGTH);
                    if (retVal != 0) {
                        safuLogErrErrnoValue("bind failed", retVal);
                    } else {
                        result = SAFU_RESULT_OK;
                    }
                }
            }
        }
    }

    return result;
}

static safuResultE_t _initializeConnections(elosClientManager_t *clientmanager, samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i += 1) {
        elosClientManagerConnection_t *connection = &clientmanager->connection[i];
        int retVal;

        retVal = pthread_mutex_init(&connection->lock, NULL);
        if (retVal != 0) {
            safuLogErrF("pthread_mutex_init failed on slot:%d; returned:%d / %s", i, retVal, strerror(errno));
            break;
        } else {
            safuResultE_t iterResult;

            connection->sharedData = &clientmanager->sharedData;
            connection->status = 0;
            connection->fd = -1;

            // TODO: Does this really have to be initialized 200+ times?! Looks more like a sharedData entry
            connection->isTrusted = false;
            iterResult = elosBlacklistInitialize(&connection->blacklist, config);
            if (iterResult == SAFU_RESULT_FAILED) {
                safuLogWarn("blacklist initialization failed");
            }
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

    // TODO: Detect multiple uses of elosClientManagerStart properly?

    if ((clientmanager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if ((param->config == NULL) || (param->eventDispatcher == NULL) || (param->eventProcessor == NULL) ||
               (param->logAggregator == NULL)) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else {
        int retVal;

        memset(clientmanager, 0, sizeof(elosClientManager_t));

        retVal = pthread_mutex_init(&clientmanager->lock, NULL);
        if (retVal != 0) {
            safuLogErrErrnoValue("pthread_mutex_init failed", retVal);
        } else {
            result = _initializeSharedData(clientmanager, param);
            if (result == SAFU_RESULT_OK) {
                result = _initializeListener(clientmanager, param->config);
                if (result == SAFU_RESULT_OK) {
                    result = _initializeConnections(clientmanager, param->config);
                    if (result == SAFU_RESULT_OK) {
                        _initializeAuthorization(clientmanager, param->config);
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientmanager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientmanager == NULL) {
        safuLogErr("Invalid parameter");
    }

    return result;
}
