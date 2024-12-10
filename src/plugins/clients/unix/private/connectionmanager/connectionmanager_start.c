// SPDX-License-Identifier: MIT

#include "connectionmanager/types.h"

safuResultE_t elosConnectionManagerStart(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connectionManager != NULL) {
        result = SAFU_RESULT_OK;
    }

    return result;
}
