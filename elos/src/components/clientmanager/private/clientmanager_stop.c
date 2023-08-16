// SPDX-License-Identifier: MIT

#include <errno.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <unistd.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientauthorization.h"
#include "elos/clientmanager/clientauthorizedprocesses.h"
#include "elos/clientmanager/clientblacklist.h"
#include "elos/clientmanager/clientconnection.h"
#include "elos/clientmanager/clientmanager.h"

safuResultE_t elosClientManagerStop(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientManager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientManager->flags) == false) {
        safuLogErr("The given ClientManager is not initialized");
    } else {
        int retVal;

        if (atomic_load(&clientManager->flags) & CLIENT_MANAGER_LISTEN_ACTIVE) {
            safuLogDebug("Stop ClientManager worker...");
            atomic_fetch_and(&clientManager->flags, ~CLIENT_MANAGER_LISTEN_ACTIVE);
            retVal = pthread_join(clientManager->listenThread, NULL);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Joining ClientManager worker failed (possible memory leak)", retVal);
            } else {
                result = SAFU_RESULT_FAILED;
            }
            atomic_fetch_and(&clientManager->flags, ~CLIENT_MANAGER_THREAD_NOT_JOINED);
        }

        safuLogDebug("ClientManager worker stopped");
    }

    return result;
}
