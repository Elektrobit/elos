// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/clientblacklist.h>
#include <elos/libelosplugin/clientconnection.h>
#include <elos/libelosplugin/connectionmanager.h>
#include <errno.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <unistd.h>

#include "connectionmanager_private.h"

safuResultE_t elosConnectionManagerStop(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connectionManager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == false) {
        safuLogErr("The given ConnectionManager is not initialized");
    } else {
        int retVal;

        if (atomic_load(&connectionManager->flags) & ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE) {
            safuLogDebug("Stop ConnectionManager worker...");
            atomic_fetch_and(&connectionManager->flags, ~ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE);
            retVal = pthread_join(connectionManager->listenThread, NULL);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Joining ConnectionManager worker failed (possible memory leak)", retVal);
            } else {
                result = SAFU_RESULT_OK;
            }
            if (connectionManager->closeListener != NULL) {
                result = connectionManager->closeListener(connectionManager);
            } else {
                result = SAFU_RESULT_FAILED;
            }
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Closing of connection failed");
            }
            atomic_fetch_and(&connectionManager->flags, ~ELOS_CONNECTIONMANAGER_THREAD_NOT_JOINED);
        }

        safuLogDebug("ConnectionManager worker stopped");
    }

    return result;
}
