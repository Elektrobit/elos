// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <sys/eventfd.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientmanager.h"

safuResultE_t elosClientManagerStart(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;

    if (clientManager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientManager->flags) == false) {
        safuLogErr("The given ClientManager is not initialized");
    } else {
        atomic_fetch_or(&clientManager->flags, CLIENT_MANAGER_LISTEN_ACTIVE);

        retVal = pthread_create(&clientManager->listenThread, 0, elosClientManagerThreadListen, clientManager);
        if (retVal != 0) {
            safuLogErrErrnoValue("pthread_create failed", retVal);
        } else {
            eventfd_t value = 0;

            retVal = eventfd_read(clientManager->syncFd, &value);
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
