// SPDX-License-Identifier: MIT

// clang-format off
#define _GNU_SOURCE
#include <pthread.h>
// clang-format on

#include <elos/libelosplugin/connectionmanager.h>
#include <safu/log.h>
#include <sys/eventfd.h>

#include "connectionmanager_private.h"

safuResultE_t elosConnectionManagerStart(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;

    if (connectionManager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == false) {
        safuLogErr("The given ConnectionManager is not initialized");
    } else {
        atomic_fetch_or(&connectionManager->flags, ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE);

        retVal =
            pthread_create(&connectionManager->listenThread, 0, elosConnectionManagerThreadListen, connectionManager);
        if (retVal != 0) {
            safuLogErrErrnoValue("pthread_create failed", retVal);
        } else {
            retVal = pthread_setname_np(connectionManager->listenThread, "ConManager");
            if (retVal != 0) {
                safuLogErr("Failed to set thread name for connection manager");
            }

            eventfd_t value = 0;
            retVal = eventfd_read(connectionManager->syncFd, &value);
            if (retVal < 0) {
                safuLogErrErrnoValue("eventfd_read failed", retVal);
            } else {
                safuLogDebug("listen thread active");
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}
