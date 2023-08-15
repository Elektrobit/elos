// SPDX-License-Identifier: MIT

#include <safu/log.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientmanager.h"

safuResultE_t elosClientManagerStart(elosClientManager_t *clientmanager) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;

    if (clientmanager == NULL) {
        safuLogErr("Invalid parameter NULL");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&clientmanager->flags) == false) {
        safuLogErr("The given ClientManager is not initialized");
    } else {
        atomic_fetch_or(&clientmanager->flags, CLIENT_MANAGER_LISTEN_ACTIVE);

        retVal = pthread_create(&clientmanager->listenThread, 0, elosClientManagerThreadListen, clientmanager);
        if (retVal != 0) {
            safuLogErrErrnoValue("pthread_create failed", retVal);
        } else {
            safuLogDebug("listen thread active");
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
