// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <elos/libelosplugin/clientauthorization_types.h>
#include <elos/libelosplugin/clientconnection.h>
#include <elos/libelosplugin/connectionmanager.h>
#include <safu/mutex.h>
#include <sys/eventfd.h>
#include <sys/time.h>

#include "connectionmanager_private.h"

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

safuResultE_t elosConnectionManagerThreadGetFreeConnectionSlot(elosConnectionManager_t *connectionManager, int *slot) {
    sem_t *connectionSemaphore = &connectionManager->sharedData.connectionSemaphore;
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
            for (int i = 0; i < connectionManager->connectionLimit; i += 1) {
                elosClientConnection_t *connection = &connectionManager->connections[i];
                bool active = false;

                result = elosClientConnectionIsActive(connection, &active);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrF("elosClientConnectionIsTaken failed for connection slot:%d", i);
                    break;
                } else if (active == false) {
                    *slot = i;
                    break;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosConnectionManagerThreadWaitForIncomingConnection(elosConnectionManager_t *connectionManager,
                                                                   int slot) {
    struct timespec timeOut = {.tv_sec = CONNECTION_PSELECT_TIMEOUT_SEC, .tv_nsec = CONNECTION_PSELECT_TIMEOUT_NSEC};
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnection_t *connection = &connectionManager->connections[slot];

    for (;;) {
        int retval;
        fd_set readFds;

        FD_ZERO(&readFds);
        FD_SET(connectionManager->fd, &readFds);

        if (!(atomic_load(&connectionManager->flags) & ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE)) {
            break;
        }

        retval = pselect((connectionManager->fd + 1), &readFds, NULL, NULL, &timeOut, NULL);
        if (retval < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                safuLogErrErrno("pselect failed!");
                result = SAFU_RESULT_FAILED;
                break;
            }
        } else if (retval > 0) {
            if (!FD_ISSET(connectionManager->fd, &readFds)) {
                safuLogWarn("pselect returned with value > 0, but the selected fd has no new data!");
                continue;
            }
        } else {
            continue;
        }

        result = connectionManager->accept(connectionManager, connection);

        if (result == SAFU_RESULT_OK) {
            safuLogDebug("Accepted new connection");

            result = connectionManager->authorize(connectionManager, connection);
            if (connection->isTrusted) {
                safuLogDebug("connection is trusted");
            } else {
                safuLogDebug("connection is not trusted");
            }

            break;
        }
    }

    return result;
}

void *elosConnectionManagerThreadListen(void *ptr) {
    elosConnectionManager_t *connectionManager = (elosConnectionManager_t *)ptr;
    bool active = false;
    int retVal;

    retVal = eventfd_write(connectionManager->syncFd, _SYNCFD_VALUE);
    if (retVal < 0) {
        safuLogErrErrnoValue("eventfd_write failed", retVal);
    } else {
        atomic_fetch_or(&connectionManager->flags, ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE);
        active = true;
    }

    while (active == true) {
        elosClientConnection_t *connection = NULL;
        safuResultE_t result = SAFU_RESULT_FAILED;
        int slot = -1;

        // wait until we have a free connection slot available
        result = elosConnectionManagerThreadGetFreeConnectionSlot(connectionManager, &slot);
        if (result != SAFU_RESULT_OK) {
            break;
        } else if (slot < 0) {
            continue;
        } else {
            connection = &connectionManager->connections[slot];

            result = elosConnectionManagerThreadWaitForIncomingConnection(connectionManager, slot);
            if (result != SAFU_RESULT_OK) {
                SAFU_SEM_UNLOCK(&connectionManager->sharedData.connectionSemaphore, break);
            } else {
                result = elosClientConnectionStart(connection);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Starting client connection failed");
                    continue;
                }
            }
        }

        if (!(atomic_load(&connectionManager->flags) & ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE)) {
            active = false;
        }
    }

    atomic_fetch_and(&connectionManager->flags, ~ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE);
    atomic_fetch_or(&connectionManager->flags, ELOS_CONNECTIONMANAGER_THREAD_NOT_JOINED);

    return NULL;
}
