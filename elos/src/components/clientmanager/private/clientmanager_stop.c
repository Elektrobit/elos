// SPDX-License-Identifier: MIT

#include <errno.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <unistd.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientauthorizedprocesses.h"
#include "elos/clientmanager/clientblacklist.h"
#include "elos/clientmanager/clientmanager.h"

safuResultE_t elosClientManagerGetStatus(elosClientManager_t *context, uint32_t *status) {
    safuResultE_t result = SAFU_RESULT_OK;

    SAFU_PTHREAD_MUTEX_LOCK(&context->lock, result = SAFU_RESULT_FAILED);
    if (result != SAFU_RESULT_FAILED) {
        *status = context->status;
        SAFU_PTHREAD_MUTEX_UNLOCK(&context->lock, result = SAFU_RESULT_FAILED);
    }
    return result;
}

static void _stopListeningThread(elosClientManager_t *ctx) {
    safuLogDebug("stop listening thread...");
    pthread_mutex_lock(&ctx->lock);
    ctx->status &= ~CLIENT_MANAGER_LISTEN_ACTIVE;
    pthread_mutex_unlock(&ctx->lock);
    pthread_join(ctx->listenThread, NULL);
    pthread_mutex_destroy(&ctx->lock);
    ctx->status &= ~CLIENT_MANAGER_THREAD_NOT_JOINED;
}

static void _stopConnectionThreads(elosClientManager_t *ctx) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i += 1) {
        pthread_mutex_lock(&ctx->connection[i].lock);
        result = elosBlacklistDelete(&ctx->connection[i].blacklist);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("not able to delete blacklist filter");
        }
        if (ctx->connection[i].status & CLIENT_MANAGER_CONNECTION_ACTIVE) {
            safuLogInfoF("stop connection thread '%d'", i);
            ctx->connection[i].status &= ~CLIENT_MANAGER_CONNECTION_ACTIVE;
            pthread_mutex_unlock(&ctx->connection[i].lock);
            pthread_join(ctx->connection[i].thread, NULL);
            pthread_mutex_destroy(&ctx->connection[i].lock);
            ctx->connection[i].status &= ~CLIENT_MANAGER_THREAD_NOT_JOINED;
        } else {
            pthread_mutex_unlock(&ctx->connection[i].lock);
        }
    }
}

safuResultE_t elosClientManagerStop(elosClientManager_t *clientmanager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientmanager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else {
        uint32_t status = 0;

        result = elosClientManagerGetStatus(clientmanager, &status);
        if (result == SAFU_RESULT_OK) {
            int retVal;

            if (status & CLIENT_MANAGER_LISTEN_ACTIVE) {
                _stopListeningThread(clientmanager);
            }

            _stopConnectionThreads(clientmanager);

            retVal = sem_destroy(&clientmanager->sharedData.connectionSemaphore);
            if (retVal != 0) {
                safuLogWarnF("sem_destroy failed! - returned:%d, %s", retVal, strerror(errno));
            }

            elosClientAuthorizationDelete(&clientmanager->clientAuth);

            if (clientmanager->fd != -1) {
                retVal = close(clientmanager->fd);
                if (retVal != 0) {
                    safuLogWarnF("close failed! - returned:%d, %s", retVal, strerror(errno));
                }
            }

            safuLogDebug("done");
        }
    }

    return result;
}
