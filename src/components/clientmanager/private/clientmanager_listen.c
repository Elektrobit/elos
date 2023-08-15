// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <safu/mutex.h>
#include <sys/time.h>
#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientmanager.h"

#define CONNECTION_SEMAPHORE_TIMEOUT_SEC  0
#define CONNECTION_SEMAPHORE_TIMEOUT_NSEC (100 * 1000 * 1000)
#define CONNECTION_PSELECT_TIMEOUT_SEC    0
#define CONNECTION_PSELECT_TIMEOUT_NSEC   (100 * 1000 * 1000)
#define TIMESPEC_1SEC_IN_NSEC             1000000000L

static inline void _calculateTvSeconds(struct timespec *ts) {
    struct timespec connSemTimeOut = {.tv_sec = CONNECTION_SEMAPHORE_TIMEOUT_SEC,
                                      .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC};
    struct timeval tmpTs, tmpTimeOut, tmpResult;
    TIMESPEC_TO_TIMEVAL(&tmpTs, ts);
    TIMESPEC_TO_TIMEVAL(&tmpTimeOut, &connSemTimeOut);

    timeradd(&tmpTs, &tmpTimeOut, &tmpResult);
    TIMEVAL_TO_TIMESPEC(&tmpResult, ts);
}

safuResultE_t elosClientManagerThreadGetFreeConnectionSlot(elosClientManager_t *ctx, int *slot) {
    sem_t *connectionSemaphore = &ctx->sharedData.connectionSemaphore;
    struct timespec semTimeOut = {0};
    safuResultE_t result = SAFU_RESULT_OK;

    *slot = -1;

    int retval = clock_gettime(CLOCK_REALTIME, &semTimeOut);
    if (retval < 0) {
        safuLogCritF("clock_gettime failed! - %s", strerror(errno));
        result = SAFU_RESULT_FAILED;
        raise(SIGTERM);
    } else {
        _calculateTvSeconds(&semTimeOut);
        retval = sem_timedwait(connectionSemaphore, &semTimeOut);
        if (retval < 0) {
            if ((errno == ETIMEDOUT) || (errno == EINTR)) {
                safuLogInfoF("sem_timedwait stopped - %s", strerror(errno));
            } else {
                safuLogCritF("sem_timedwait failed! - %s", strerror(errno));
                result = SAFU_RESULT_FAILED;
                raise(SIGTERM);
            }
        } else {
            for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i += 1) {
                elosClientManagerConnection_t *conn = &ctx->connection[i];
                SAFU_PTHREAD_MUTEX_LOCK(&conn->lock, result = SAFU_RESULT_FAILED);
                if (result == SAFU_RESULT_FAILED) {
                    break;
                }
                if (conn->status & CLIENT_MANAGER_CONNECTION_ACTIVE) {
                    SAFU_PTHREAD_MUTEX_UNLOCK(&conn->lock, result = SAFU_RESULT_FAILED);
                    if (result == SAFU_RESULT_FAILED) {
                        break;
                    }
                    continue;
                }
                if (conn->status & CLIENT_MANAGER_THREAD_NOT_JOINED) {
                    pthread_join(conn->thread, NULL);
                    conn->status &= ~CLIENT_MANAGER_THREAD_NOT_JOINED;
                }
                SAFU_PTHREAD_MUTEX_UNLOCK(&conn->lock, result = SAFU_RESULT_FAILED);
                if (result == SAFU_RESULT_FAILED) {
                    break;
                }
                *slot = i;
                break;
            }
        }
    }
    return result;
}

safuResultE_t elosClientManagerThreadWaitForIncomingConnection(elosClientManager_t *ctx, int slot) {
    elosClientManagerConnection_t *conn = &ctx->connection[slot];
    struct timespec timeOut = {.tv_sec = CONNECTION_PSELECT_TIMEOUT_SEC, .tv_nsec = CONNECTION_PSELECT_TIMEOUT_NSEC};
    socklen_t addrLen = sizeof(conn->addr);
    safuResultE_t result = SAFU_RESULT_FAILED;

    for (;;) {
        int retval;
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(ctx->fd, &readFds);

        if (!(atomic_load(&ctx->flags) & CLIENT_MANAGER_LISTEN_ACTIVE)) {
            break;
        }

        retval = pselect((ctx->fd + 1), &readFds, NULL, NULL, &timeOut, NULL);
        if (retval < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                safuLogErrErrno("pselect failed!");
                result = SAFU_RESULT_FAILED;
                break;
            }
        } else if (retval > 0) {
            if (!FD_ISSET(ctx->fd, &readFds)) {
                safuLogWarn("pselect returned with value > 0, but the selected fd has no new data!");
                continue;
            }
        } else {
            continue;
        }

        conn->fd = accept(ctx->fd, (struct sockaddr *)&conn->addr, &addrLen);
        if (conn->fd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                safuLogErrErrno("accept failed!");
                result = SAFU_RESULT_FAILED;
                break;
            }
        } else {
            result = SAFU_RESULT_OK;
        }

        safuLogDebug("Accepted new connection");
        conn->isTrusted = elosClientAuthorizationIsTrustedConnection(&ctx->clientAuth, &conn->addr);
        if (conn->isTrusted) {
            safuLogDebug("connection is trusted");
        } else {
            safuLogDebug("connection is not trusted");
        }
        break;
    }

    return result;
}

void *elosClientManagerThreadListen(void *ptr) {
    elosClientManager_t *ctx = (elosClientManager_t *)ptr;
    safuResultE_t result = SAFU_RESULT_FAILED;

    for (;;) {
        elosClientManagerConnection_t *connection = NULL;
        int slot;

        if (!(atomic_load(&ctx->flags) & CLIENT_MANAGER_LISTEN_ACTIVE)) {
            break;
        }

        // wait until we have a free connection slot available
        result = elosClientManagerThreadGetFreeConnectionSlot(ctx, &slot);
        if (result != SAFU_RESULT_OK) {
            break;
        } else if (slot < 0) {
            continue;
        } else {
            connection = &ctx->connection[slot];
        }

        SAFU_PTHREAD_MUTEX_LOCK(&connection->lock, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_FAILED) {
            break;
        }

        result = elosClientManagerThreadWaitForIncomingConnection(ctx, slot);
        if (result != SAFU_RESULT_OK) {
            SAFU_PTHREAD_MUTEX_UNLOCK(&connection->lock, break);
            SAFU_SEM_UNLOCK(&ctx->sharedData.connectionSemaphore, break);
            continue;
        }

        // send and receive messages over each connection in a separate thread
        int retval = pthread_create(&connection->thread, 0, elosClientManagerThreadConnection, (void *)connection);
        if (retval != 0) {
            safuLogErrErrno("pthread_create failed");
            result = SAFU_RESULT_FAILED;
            close(connection->fd);
            connection->fd = -1;
            connection->status &= ~CLIENT_MANAGER_CONNECTION_ACTIVE;
            SAFU_SEM_UNLOCK(&ctx->sharedData.connectionSemaphore, break);
        } else {
            connection->status |= CLIENT_MANAGER_CONNECTION_ACTIVE;
        }

        SAFU_PTHREAD_MUTEX_UNLOCK(&connection->lock, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_FAILED) {
            break;
        }
    }

    safuLogDebug("closing...");
    atomic_fetch_and(&ctx->flags, ~CLIENT_MANAGER_LISTEN_ACTIVE);
    atomic_fetch_or(&ctx->flags, CLIENT_MANAGER_THREAD_NOT_JOINED);

    return (void *)result;
}
