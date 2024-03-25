// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <errno.h>
#include <poll.h>
#include <safu/log.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/connectionmanager/clientconnection.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/messages/message_handler.h"

#if EAGAIN != EWOULDBLOCK
#define _CASE_EWOULDBLOCK case EWOULDBLOCK:
#else
#define _CASE_EWOULDBLOCK
#endif

#define _SYNCFD_VALUE 1

static safuResultE_t _workerDataInitialize(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionData_t *workerData = &clientConnection->data;
    uint32_t const eqIdElements = ELOS_CONNECTIONMANAGER_EVENTQUEUEIDVECTOR_SIZE;
    uint32_t const efnIdElements = ELOS_CONNECTIONMANAGER_EVENTFILTERNODEIDVECTOR_SIZE;
    int retVal;

    retVal = safuVecCreate(&workerData->eventQueueIdVector, eqIdElements, sizeof(elosEventQueueId_t));
    if (retVal != 0) {
        safuLogErrValue("Creating EventQueueId vector failed", retVal);
    } else {
        retVal = safuVecCreate(&workerData->eventFilterNodeIdVector, efnIdElements, sizeof(elosEventFilterNodeId_t));
        if (retVal != 0) {
            safuLogErrValue("Creating EventFilterNodeId vector failed", retVal);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t _workerDataDeleteMembers(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionData_t *workerData = &clientConnection->data;
    elosEventProcessor_t *eventProcessor = clientConnection->sharedData->eventProcessor;
    uint32_t elements;
    int retVal;

    elements = safuVecElements(&workerData->eventFilterNodeIdVector);
    for (uint32_t idx = 0; idx < elements; idx += 1) {
        elosEventFilterNodeId_t *id;

        id = safuVecGet(&workerData->eventFilterNodeIdVector, idx);
        if (id == NULL) {
            safuLogWarnF("safuVecGet with 'idx:%u' failed", idx);
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventProcessorFilterNodeRemove(eventProcessor, *id);
            if (result != SAFU_RESULT_OK) {
                safuLogWarnF("elosEventProcessorFilterNodeRemove with 'idx:%u, id:%u' failed", idx, *id);
            }
        }
    }

    retVal = safuVecFree(&workerData->eventFilterNodeIdVector);
    if (retVal < 0) {
        safuLogWarn("safuVecFree failed");
        result = SAFU_RESULT_FAILED;
    }

    elements = safuVecElements(&workerData->eventQueueIdVector);
    for (uint32_t idx = 0; idx < elements; idx += 1) {
        elosEventFilterNodeId_t *id;

        id = safuVecGet(&workerData->eventQueueIdVector, idx);
        if (id == NULL) {
            safuLogErrF("safuVecGet with 'idx:%u' failed", idx);
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventProcessorQueueRemove(eventProcessor, *id);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("elosEventProcessorQueueRemove with 'idx:%u, id:%u' failed", idx, *id);
            }
        }
    }

    retVal = safuVecFree(&workerData->eventQueueIdVector);
    if (retVal < 0) {
        safuLogWarn("safuVecFree failed");
        result = SAFU_RESULT_FAILED;
    }

    if (clientConnection->fd != -1) {
        retVal = close(clientConnection->fd);
        if (retVal != 0) {
            safuLogWarnErrnoValue("Closing socketFd failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

void *elosClientConnectionWorker(void *ptr) {
    elosClientConnection_t *clientConnection = (elosClientConnection_t *)ptr;
    safuResultE_t result;
    int retVal;
    bool active = false;
    struct timespec const timeout = ELOS_CLIENTCONNECTION_WORKER_POLL_TIMEOUT;
    struct pollfd pollFdSet[] = {
        {.fd = clientConnection->triggerFd, .events = POLLIN},
        {.fd = clientConnection->fd, .events = POLLIN},
    };

    atomic_fetch_or(&clientConnection->flags, ELOS_CLIENTCONNECTION_ACTIVE_BIT);

    retVal = eventfd_write(clientConnection->syncFd, _SYNCFD_VALUE);
    if (retVal < 0) {
        safuLogErrErrnoValue("eventfd_read failed", retVal);
    } else {
        result = _workerDataInitialize(clientConnection);
        if (result == SAFU_RESULT_OK) {
            active = true;
        }
    }

    while (active == true) {
        retVal = ppoll(pollFdSet, ARRAY_SIZE(pollFdSet), &timeout, NULL);
        if (retVal < 0) {
            switch (errno) {
                _CASE_EWOULDBLOCK
                case EAGAIN:
                case EINTR:
                    break;
                default:
                    safuLogErrErrnoValue("ppoll failed", retVal);
                    active = false;
                    break;
            }
        } else if (retVal > 0) {
            if (pollFdSet[0].revents != 0) {
                eventfd_t value = 0;

                active = false;
                if (pollFdSet[0].revents & POLLIN) {
                    retVal = eventfd_read(clientConnection->triggerFd, &value);
                    if (retVal < 0) {
                        safuLogErrErrnoValue("eventfd_read failed", retVal);
                    }
                }

            } else if (pollFdSet[1].revents & (POLLHUP | POLLERR)) {
                active = false;
            } else if (pollFdSet[1].revents & POLLIN) {
                result = elosMessageHandlerHandleMessage(clientConnection);
                if (result != SAFU_RESULT_OK) {
                    active = false;
                }
            }
        }

        if (ELOS_CLIENTCONNECTION_HAS_ACTIVE_BIT(&clientConnection->flags) == false) {
            active = false;
        }
    }

    _workerDataDeleteMembers(clientConnection);

    atomic_fetch_and(&clientConnection->flags, ~ELOS_CLIENTCONNECTION_ACTIVE_BIT);

    retVal = sem_post(&clientConnection->sharedData->connectionSemaphore);
    if (retVal != 0) {
        safuLogErrErrnoValue("sem_post failed", retVal);
    }

    return NULL;
}
