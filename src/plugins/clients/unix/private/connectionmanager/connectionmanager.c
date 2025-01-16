// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/types.h>
#include <samconf/samconf.h>
#include <stdlib.h>

#include "connectionmanager/types.h"

safuResultE_t elosConnectionManagerInitialize(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((connectionManager != NULL) || (plugin != NULL)) {
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosConnectionManagerDeleteMembers(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connectionManager != NULL) {
        result = SAFU_RESULT_OK;
    }

    return result;
}
