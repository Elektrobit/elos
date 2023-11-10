// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include "elos/connectionmanager/clientconnection.h"

#include <errno.h>
#include <limits.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "elos/connectionmanager/clientauthorization.h"
#include "elos/connectionmanager/clientblacklist.h"
#include "elos/connectionmanager/connectionmanager.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/messages/message_handler.h"

#define _TRIGGERFD_VALUE 1
#define _TRIGGERFD_SIZE  (int)sizeof(eventfd_t)
#define _SYNCFD_VALUE    _TRIGGERFD_VALUE
#define _SYNCFD_SIZE     _TRIGGERFD_SIZE

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
        memset(clientConnection, 0, sizeof(elosClientConnection_t));

        clientConnection->sharedData = param->sharedData;
        clientConnection->fd = -1;

        clientConnection->syncFd = eventfd(0, 0);
        if (clientConnection->syncFd == -1) {
            safuLogErrErrnoValue("eventfd creation (sync) failed", clientConnection->syncFd);
        } else {
            clientConnection->triggerFd = eventfd(0, 0);
            if (clientConnection->triggerFd == -1) {
                safuLogErrErrnoValue("eventfd creation (trigger) failed", clientConnection->triggerFd);
            } else {
                elosClientConnectionSharedData_t *sharedData = clientConnection->sharedData;
                elosEventBufferParam_t eventBufferParam = {
                    .limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT,
                };

                result = elosEventBufferInitialize(&clientConnection->eventBuffer, &eventBufferParam);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrValue("Creating EventBuffer failed", result);
                } else {
                    result = elosEventDispatcherBufferAdd(sharedData->eventDispatcher, &clientConnection->eventBuffer);
                    if (result != SAFU_RESULT_OK) {
                        safuLogErrValue("Adding EventBuffer to EventDispatcher failed", result);
                    } else {
                        safuResultE_t subResult;

                        clientConnection->isTrusted = false;
                        subResult = elosBlacklistInitialize(&clientConnection->blacklist, sharedData->config);
                        if (subResult == SAFU_RESULT_FAILED) {
                            safuLogWarn("blacklist initialization failed");
                        }

                        atomic_store(&clientConnection->flags, SAFU_FLAG_INITIALIZED_BIT);
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosClientConnectionDeleteMembers(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientConnection != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == true) {
            safuResultE_t stepResult;
            int retVal;

            if (ELOS_CLIENTCONNECTION_HAS_TAKEN_BIT(&clientConnection->flags) == true) {
                stepResult = elosClientConnectionStop(clientConnection);
                if (stepResult != SAFU_RESULT_OK) {
                    safuLogWarn("Stopping ClientConnection failed (possible memory leak)");
                    result = SAFU_RESULT_FAILED;
                }
            }

            stepResult = elosEventDispatcherBufferRemove(clientConnection->sharedData->eventDispatcher,
                                                         &clientConnection->eventBuffer);
            if (stepResult != SAFU_RESULT_OK) {
                safuLogWarn("Removing EventBuffer from EventDispatcher failed");
                result = SAFU_RESULT_FAILED;
            }

            stepResult = elosEventBufferDeleteMembers(&clientConnection->eventBuffer);
            if (stepResult != SAFU_RESULT_OK) {
                safuLogWarn("Deleting EventBuffer members failed");
                result = SAFU_RESULT_FAILED;
            }

            stepResult = elosBlacklistDelete(&clientConnection->blacklist);
            if (stepResult != SAFU_RESULT_OK) {
                safuLogWarn("Deleting Blacklist members failed");
                result = SAFU_RESULT_FAILED;
            }

            if (clientConnection->syncFd != -1) {
                retVal = close(clientConnection->syncFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (clientConnection->triggerFd != -1) {
                retVal = close(clientConnection->triggerFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosClientConnectionStart(elosClientConnection_t *clientConnection, int socketFd) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientConnection == NULL) || (socketFd == -1)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientConnection->flags) == false) {
        safuLogErr("The given ClientConnection is not initialized");
    } else if (ELOS_CLIENTCONNECTION_HAS_ACTIVE_BIT(&clientConnection->flags) == true) {
        safuLogErr("The given ClientConnection is already active");
    } else {
        int retVal;

        if (ELOS_CLIENTCONNECTION_HAS_TAKEN_BIT(&clientConnection->flags) == true) {
            result = elosClientConnectionStop(clientConnection);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Resetting already used connection failed");
            }
        }

        clientConnection->fd = socketFd;

        retVal = pthread_create(&clientConnection->thread, 0, elosClientConnectionWorker, (void *)clientConnection);
        if (retVal != 0) {
            safuLogErrErrnoValue("Worker thread creation failed", retVal);
        } else {
            eventfd_t value = 0;

            retVal = eventfd_read(clientConnection->syncFd, &value);
            if (retVal < 0) {
                safuLogErrErrnoValue("eventfd_read failed", retVal);
            } else {
                atomic_fetch_or(&clientConnection->flags, ELOS_CLIENTCONNECTION_TAKEN_BIT);
                result = SAFU_RESULT_OK;
            }
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
        int retVal;

        result = SAFU_RESULT_OK;

        if (ELOS_CLIENTCONNECTION_HAS_ACTIVE_BIT(&clientConnection->flags) == true) {
            atomic_fetch_and(&clientConnection->flags, ~ELOS_CLIENTCONNECTION_ACTIVE_BIT);
            retVal = eventfd_write(clientConnection->triggerFd, _TRIGGERFD_VALUE);
            if (retVal != _TRIGGERFD_SIZE) {
                safuLogErrErrnoValue("eventfd_write failed", retVal);
                result = SAFU_RESULT_FAILED;
            }
        }

        if (ELOS_CLIENTCONNECTION_HAS_TAKEN_BIT(&clientConnection->flags) == true) {
            retVal = pthread_join(clientConnection->thread, NULL);
            if (retVal != 0) {
                safuLogWarnF("Stopping ClientConnection worker thread failed (returned: %d)", retVal);
                result = SAFU_RESULT_FAILED;
            } else {
                atomic_fetch_and(&clientConnection->flags, ~ELOS_CLIENTCONNECTION_TAKEN_BIT);
            }
        }
    }

    return result;
}

safuResultE_t elosClientConnectionIsActive(elosClientConnection_t *clientConnection, bool *active) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientConnection == NULL) || (active == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else {
        *active = ELOS_CLIENTCONNECTION_HAS_ACTIVE_BIT(&clientConnection->flags);
        result = SAFU_RESULT_OK;
    }

    return result;
}
