// SPDX-License-Identifier: MIT

#include <errno.h>
#include <unistd.h>

#include "elos/client_manager/client_authorization.h"
#include "elos/client_manager/client_authorized_processes.h"
#include "elos/client_manager/client_blacklist.h"
#include "elos/client_manager/client_manager.h"
#include "safu/log.h"

safuResultE_t elosClientManagerGetStatus(elosClientManagerContext_t *context, uint32_t *status) {
    safuResultE_t result = SAFU_RESULT_OK;

    SAFU_PTHREAD_MUTEX_LOCK(&context->lock, result = SAFU_RESULT_FAILED);
    if (result != SAFU_RESULT_FAILED) {
        *status = context->status;
        SAFU_PTHREAD_MUTEX_UNLOCK(&context->lock, result = SAFU_RESULT_FAILED);
    }
    return result;
}

static void _stopListeningThread(elosClientManagerContext_t *ctx) {
    safuLogDebug("stop listening thread...");
    pthread_mutex_lock(&ctx->lock);
    ctx->status &= ~CLIENT_MANAGER_LISTEN_ACTIVE;
    pthread_mutex_unlock(&ctx->lock);
    pthread_join(ctx->listenThread, NULL);
    pthread_mutex_destroy(&ctx->lock);
    ctx->status &= ~CLIENT_MANAGER_THREAD_NOT_JOINED;
}

static void _stopConnectionThreads(elosClientManagerContext_t *ctx) {
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

int elosClientManagerStop(elosClientManagerContext_t *ctx) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    uint32_t status = 0;
    int retval = -1;

    if (ctx == NULL) {
        safuLogErr("Called elosClientManagerStop with parameter being NULL");
    } else {
        result = elosClientManagerGetStatus(ctx, &status);
    }

    if (result == SAFU_RESULT_OK) {
        if (status & CLIENT_MANAGER_LISTEN_ACTIVE) {
            _stopListeningThread(ctx);
        }

        _stopConnectionThreads(ctx);

        if (sem_destroy(&ctx->sharedData.connectionSemaphore) < 0) {
            safuLogWarnF("sem_destroy failed! - %s", strerror(errno));
        }

        elosClientAuthorizationDelete(&ctx->clientAuth);

        close(ctx->fd);

        safuLogDebug("done");
        retval = 0;
    }

    return retval;
}
