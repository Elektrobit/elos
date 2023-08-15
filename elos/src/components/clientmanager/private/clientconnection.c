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
#include "elos/clientmanager/clientmanager.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/messages/message_handler.h"
#include "elos/clientmanager/clientblacklist.h"

#include "elos/clientmanager/clientconnection.h"

safuResultE_t _getStatus(elosClientConnection_t *connection, uint32_t *status) {
    safuResultE_t result = SAFU_RESULT_OK;

    SAFU_PTHREAD_MUTEX_LOCK(&connection->lock, result = SAFU_RESULT_FAILED);
    if (result != SAFU_RESULT_FAILED) {
        *status = connection->status;
        SAFU_PTHREAD_MUTEX_UNLOCK(&connection->lock, result = SAFU_RESULT_FAILED);
    }

    return result;
}

static safuResultE_t _createConnectionData(elosClientConnection_t *conn) {
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

safuResultE_t elosClientConnectionInitialize(elosClientConnection_t *clientConnection,
                                             elosClientConnectionParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientConnection == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if (param->sharedData == NULL) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == true) {
        safuLogErr("The given ClientConnection is already initialized");
    } else {
        int retVal;

        memset(clientConnection, 0, sizeof(elosClientConnection_t));

        retVal = pthread_mutex_init(&clientConnection->lock, NULL);
        if (retVal != 0) {
            safuLogErrErrnoValue("Mutex intialization failed", retVal);
        } else {
            safuResultE_t subResult;

            clientConnection->sharedData = param->sharedData;
            clientConnection->fd = -1;

            clientConnection->isTrusted = false;
            subResult = elosBlacklistInitialize(&clientConnection->blacklist, clientConnection->sharedData->config);
            if (subResult == SAFU_RESULT_FAILED) {
                safuLogWarn("blacklist initialization failed");
            }

            atomic_store(&clientConnection->flags, SAFU_FLAG_INITIALIZED_BIT);
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static safuResultE_t _cleanupConnectionData(elosClientConnection_t *conn) {
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

static void _closeConnection(elosClientConnection_t *conn) {
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

safuResultE_t elosClientConnectionDeleteMembers(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientConnection != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == true) {
            _closeConnection(clientConnection);
        }
    }

    return result;
}

void *elosClientConnectionWorker(void *ptr) {
    elosClientConnection_t *connection = (elosClientConnection_t *)ptr;
    safuResultE_t result = SAFU_RESULT_OK;

    connection->status |= CLIENT_MANAGER_CONNECTION_ACTIVE;

    result = _createConnectionData(connection);
    // send and receive messages over the connection
    while (result == SAFU_RESULT_OK) {
        uint32_t status = 0;

        result = _getStatus(connection, &status);
        if (result == SAFU_RESULT_OK) {
            if (status & ELOS_CLIENTCONNECTION_ACTIVE) {
                result = elosMessageHandlerHandleMessage(connection);
            } else {
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    _closeConnection(connection);
    return NULL;
}

safuResultE_t elosClientConnectionStart(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientConnection == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == false) {
        safuLogErr("The given ClientConnection is not initialized");
    } else {
        int retVal;

        retVal = pthread_create(&clientConnection->thread, 0, elosClientConnectionWorker, (void *)clientConnection);
        if (retVal != 0) {
            safuLogErrErrno("pthread_create failed");
            close(clientConnection->fd);
            clientConnection->fd = -1;
            clientConnection->status &= ~CLIENT_MANAGER_CONNECTION_ACTIVE;
            SAFU_SEM_UNLOCK(&(clientConnection->sharedData->connectionSemaphore), result = SAFU_RESULT_FAILED);
        } else {
            clientConnection->status |= CLIENT_MANAGER_CONNECTION_ACTIVE;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosClientConnectionStop(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientConnection == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == false) {
        safuLogErr("The given ClientConnection is not initialized");
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}
