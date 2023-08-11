// SPDX-License-Identifier: MIT

#include "elos/clientmanager/clientmanager.h"

#include <safu/log.h>

#include "clientmanager_private.h"

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientmanager, elosClientManagerParam_t *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((clientmanager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        memset(clientmanager, 0, sizeof(elosClientManager_t));
    }

    return result;
}

safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientmanager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientmanager == NULL) {
        safuLogErr("Invalid parameter");
    }

    return result;
}
