// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <elos/libelosplugin/clientconnection.h>
#include <elos/libelosplugin/clientconnection_defines.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <elos/libelosplugin/message_handler.h>
#include <errno.h>
#include <poll.h>
#include <safu/log.h>
#include <sys/eventfd.h>
#include <unistd.h>

#if EAGAIN != EWOULDBLOCK
#define _CASE_EWOULDBLOCK case EWOULDBLOCK:
#else
#define _CASE_EWOULDBLOCK
#endif

#define _SYNCFD_VALUE 1

static safuResultE_t _workerDataInitialize(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosPluginCreatePublisher(clientConnection->sharedData->plugin, &clientConnection->data.publisher);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("Failed to create publisher for client");
    } else {
        result = elosPluginCreateSubscriber(clientConnection->sharedData->plugin, &clientConnection->data.subscriber);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed to create subscriber for client");
        }
    }

    return result;
}

safuResultE_t _workerDataDeleteMembers(elosClientConnection_t *clientConnection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosPluginUnsubscribeAll(clientConnection->sharedData->plugin, clientConnection->data.subscriber);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("cleanup subscriptions failed");
    }

    if (clientConnection->closeConnection != NULL) {
        result = clientConnection->closeConnection(clientConnection);
    }
    if (result != SAFU_RESULT_OK) {
        safuLogErr("Closing connection failed");
    }

    result = elosPluginDeletePublisher(clientConnection->sharedData->plugin, clientConnection->data.publisher);
    result = elosPluginDeleteSubscriber(clientConnection->sharedData->plugin, clientConnection->data.subscriber);

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
