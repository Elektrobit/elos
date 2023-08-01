// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "elos/client_manager/client_authorization.h"
#include "elos/client_manager/client_manager.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/messages/message_handler.h"
#include "safu/common.h"
#include "safu/log.h"

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

safuResultE_t elosClientManagerThreadGetFreeConnectionSlot(elosClientManagerContext_t *ctx, int *slot) {
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

safuResultE_t elosClientManagerThreadWaitForIncomingConnection(elosClientManagerContext_t *ctx, int slot) {
    elosClientManagerConnection_t *conn = &ctx->connection[slot];
    struct timespec timeOut = {.tv_sec = CONNECTION_PSELECT_TIMEOUT_SEC, .tv_nsec = CONNECTION_PSELECT_TIMEOUT_NSEC};
    socklen_t addrLen = sizeof(conn->addr);
    uint32_t status = 0;
    safuResultE_t result = SAFU_RESULT_FAILED;

    for (;;) {
        int retval;
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(ctx->fd, &readFds);

        result = elosClientManagerGetStatus(ctx, &status);
        if (result != SAFU_RESULT_OK) {
            break;
        }

        if (!(status & CLIENT_MANAGER_LISTEN_ACTIVE)) {
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

static safuResultE_t _createConnectionData(elosClientManagerConnection_t *conn) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;

    retVal = safuVecCreate(&conn->data.eventQueueIdVector, CLIENT_MANAGER_EVENTQUEUEIDVECTOR_SIZE,
                           sizeof(elosEventQueueId_t));
    if (retVal != 0) {
        safuLogErrValue("Creating EventQueueId vector failed", retVal);
    } else {
        retVal = safuVecCreate(&conn->data.eventFilterNodeIdVector, CLIENT_MANAGER_EVENTFILTERNODEIDVECTOR_SIZE,
                               sizeof(elosEventFilterNodeId_t));
        if (retVal != 0) {
            safuLogErrValue("Creating EventFilterNodeId vector failed", retVal);
        } else {
            elosEventBufferParam_t param = {
                .limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT,
            };

            result = elosEventBufferInitialize(&conn->eventBuffer, &param);
            if (result != SAFU_RESULT_OK) {
                safuLogErrValue("Creating EventBuffer failed", result);
            } else {
                result = elosEventDispatcherBufferAdd(conn->sharedData->eventDispatcher, &conn->eventBuffer);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrValue("Adding EventBuffer to EventDispatcher failed", result);
                }
            }
        }
    }

    return result;
}

static safuResultE_t _cleanupConnectionData(elosClientManagerConnection_t *conn) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuResultE_t stepResult;
    uint32_t elements;
    int retVal;

    elements = safuVecElements(&conn->data.eventFilterNodeIdVector);
    for (uint32_t idx = 0; idx < elements; idx += 1) {
        elosEventFilterNodeId_t *id;

        id = safuVecGet(&conn->data.eventFilterNodeIdVector, idx);
        if (id == NULL) {
            safuLogWarnF("safuVecGet with 'idx:%u' failed", idx);
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventProcessorFilterNodeRemove(conn->sharedData->eventProcessor, *id);
            if (result != SAFU_RESULT_OK) {
                safuLogWarnF("elosEventProcessorFilterNodeRemove with 'idx:%u, id:%u' failed", idx, *id);
            }
        }
    }

    elements = safuVecElements(&conn->data.eventQueueIdVector);
    for (uint32_t idx = 0; idx < elements; idx += 1) {
        elosEventFilterNodeId_t *id;

        id = safuVecGet(&conn->data.eventQueueIdVector, idx);
        if (id == NULL) {
            safuLogErrF("safuVecGet with 'idx:%u' failed", idx);
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventProcessorQueueRemove(conn->sharedData->eventProcessor, *id);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("elosEventProcessorQueueRemove with 'idx:%u, id:%u' failed", idx, *id);
            }
        }
    }

    retVal = safuVecFree(&conn->data.eventFilterNodeIdVector);
    if (retVal < 0) {
        safuLogWarn("safuVecFree failed");
        result = SAFU_RESULT_FAILED;
    }

    retVal = safuVecFree(&conn->data.eventQueueIdVector);
    if (retVal < 0) {
        safuLogWarn("safuVecFree failed");
        result = SAFU_RESULT_FAILED;
    }

    stepResult = elosEventDispatcherBufferRemove(conn->sharedData->eventDispatcher, &conn->eventBuffer);
    if (stepResult != SAFU_RESULT_OK) {
        safuLogWarn("Removing EventBuffer from EventDispatcher failed");
        result = stepResult;
    }

    stepResult = elosEventBufferDeleteMembers(&conn->eventBuffer);
    if (stepResult != SAFU_RESULT_OK) {
        safuLogWarn("Deleting EventBuffer members failed");
        result = stepResult;
    }

    return result;
}

static void _closeConnection(elosClientManagerConnection_t *conn) {
    safuLogDebug("closing connection thread...");
    fflush(stdout);

    pthread_mutex_lock(&conn->lock);

    if (conn->fd >= 0) {
        close(conn->fd);
    }

    safuResultE_t result = _cleanupConnectionData(conn);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("_cleanupConnectionData failed");
    }

    conn->status &= ~CLIENT_MANAGER_CONNECTION_ACTIVE;
    conn->status |= CLIENT_MANAGER_THREAD_NOT_JOINED;

    sem_post(&conn->sharedData->connectionSemaphore);
    pthread_mutex_unlock(&conn->lock);
}

void *elosClientManagerThreadConnection(void *ptr) {
    elosClientManagerConnection_t *conn = (elosClientManagerConnection_t *)ptr;
    safuResultE_t result;

    // send and receive messages over the connection
    result = _createConnectionData(conn);
    while (result == SAFU_RESULT_OK) {
        uint32_t status = 0;

        result = elosClientManagerGetStatus((elosClientManagerContext_t *)conn, &status);
        if (result == SAFU_RESULT_OK) {
            if (status & CLIENT_MANAGER_CONNECTION_ACTIVE) {
                result = elosMessageHandlerHandleMessage(conn);
            } else {
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    _closeConnection(conn);
    pthread_exit(NULL);
}

void *elosClientManagerThreadListen(void *ptr) {
    elosClientManagerContext_t *ctx = (elosClientManagerContext_t *)ptr;
    uint32_t status = 0;
    safuResultE_t result = SAFU_RESULT_FAILED;

    for (;;) {
        elosClientManagerConnection_t *connection = NULL;
        int slot;

        result = elosClientManagerGetStatus(ctx, &status);
        if (result != SAFU_RESULT_OK) {
            break;
        }

        if (!(status & CLIENT_MANAGER_LISTEN_ACTIVE)) {
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
    pthread_mutex_lock(&ctx->lock);
    ctx->status &= ~CLIENT_MANAGER_LISTEN_ACTIVE;
    ctx->status |= CLIENT_MANAGER_THREAD_NOT_JOINED;
    pthread_mutex_unlock(&ctx->lock);
    return (void *)result;
}
