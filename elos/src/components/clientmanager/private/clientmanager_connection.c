// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <errno.h>
#include <limits.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "clientmanager_private.h"
#include "elos/clientmanager/clientmanager.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/messages/message_handler.h"

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
