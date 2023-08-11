// SPDX-License-Identifier: MIT

#include "elos/clientmanager/clientmanager.h"

#include <arpa/inet.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <string.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientauthorizedprocesses.h"
#include "elos/clientmanager/clientblacklist.h"
#include "elos/config/config.h"

static int _initContext(elosClientManager_t *ctx, elosClientManagerParam_t *parameters) {
    int retval = -1;
    memset(ctx, 0, sizeof(elosClientManager_t));

    samconfConfig_t *config = parameters->config;
    ctx->sharedData.logAggregator = parameters->logAggregator;
    ctx->sharedData.eventProcessor = parameters->eventProcessor;
    ctx->sharedData.eventDispatcher = parameters->eventDispatcher;
    ctx->addr.sin_family = AF_INET;
    ctx->addr.sin_port = htons(elosConfigGetElosdPort(config));

    int convertInterface = inet_pton(AF_INET, elosConfigGetElosdInterface(config), &(ctx->addr.sin_addr));
    if (convertInterface < 1) {
        safuLogErrErrno("inet_pton failed");
    } else {
        retval = 0;
    }

    return retval;
}

static int _initConnections(elosClientManager_t *ctx, elosClientManagerParam_t *parameters) {
    safuResultE_t status = SAFU_RESULT_OK;
    int retval = -1, contextLockInit = -1, semLockInit = -1, connectionLockInit = -1;

    contextLockInit = pthread_mutex_init(&(ctx->lock), NULL);
    if (contextLockInit < 0) {
        safuLogErrErrno("pthread_mutex_init failed");
    }

    semLockInit = sem_init(&ctx->sharedData.connectionSemaphore, 0, CLIENT_MANAGER_MAX_CONNECTIONS);
    if (semLockInit < 0) {
        safuLogErrErrno("sem_init failed!");
    }

    status = elosAuthorizedProcessInitialize(&ctx->clientAuth.authorizedProcessFilters, parameters->config);

    if (status == SAFU_RESULT_FAILED) {
        safuLogWarn("authorized process filter initialization failed");
    } else {
        safuLogDebug("authorized process filter initialization succeeded");
    }

    elosClientManagerConnection_t *conn = NULL;
    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i += 1) {
        conn = &ctx->connection[i];
        connectionLockInit = pthread_mutex_init(&conn->lock, NULL);
        if (connectionLockInit < 0) {
            safuLogErrF("pthread_mutex_init 'slot:%d'", i);
            break;
        }

        conn->isTrusted = false;
        status = elosBlacklistInitialize(&conn->blacklist, parameters->config);

        if (status == SAFU_RESULT_FAILED) {
            safuLogWarn("blacklist initialization failed");
        }

        conn->sharedData = &ctx->sharedData;
        conn->status = 0;
        conn->fd = -1;
    }

    if (contextLockInit >= 0 && semLockInit >= 0 && connectionLockInit >= 0) {
        retval = 0;
    }

    return retval;
}

static int _listenOnNewSocket(elosClientManager_t *ctx) {
    int retval = -1, sockOpt = -1, bindSock = -1, listenSock = -1;

    ctx->fd = socket(ctx->addr.sin_family, SOCK_STREAM, 0);
    if (ctx->fd < 0) {
        safuLogErrErrno("socket failed");
    } else {
        sockOpt = setsockopt(ctx->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    }

    if (sockOpt != 0) {
        safuLogErrErrno("setsocketopt SO_REUSEADDR failed");
    } else {
        bindSock = bind(ctx->fd, (struct sockaddr *)&(ctx->addr), sizeof(ctx->addr));
    }

    if (bindSock != 0) {
        safuLogErrErrno("bind failed");
    } else {
        listenSock = listen(ctx->fd, CLIENT_MANAGER_LISTEN_QUEUE_LENGTH);
    }

    if (listenSock < 0) {
        safuLogErrErrno("listen failed");
    } else {
        retval = 0;
    }

    return retval;
}

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientmanager, elosClientManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientmanager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        int initContext = -1, initThreadLocks = -1, listenOnNewSocket = -1;

        memset(clientmanager, 0, sizeof(elosClientManager_t));

        // TODO: Detect multiple uses of elosClientManagerStart properly?

        if (clientmanager == NULL || param == NULL) {
            safuLogErr("Called elosClientManagerStart with parameter being NULL");
        } else {
            initContext = _initContext(clientmanager, param);
        }

        if (initContext == 0) {
            initThreadLocks = _initConnections(clientmanager, param);
        }

        if (initThreadLocks == 0) {
            elosClientAuthorizationInitialize(&clientmanager->clientAuth);
        }

        if (initThreadLocks == 0) {
            listenOnNewSocket = _listenOnNewSocket(clientmanager);
        }

        if (listenOnNewSocket == 0) {
            result = SAFU_RESULT_OK;
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
