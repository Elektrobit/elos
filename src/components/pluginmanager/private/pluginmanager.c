// SPDX-License-Identifier: MIT

#include "elos/pluginmanager/pluginmanager.h"

#include <safu/log.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugincontrol/vector.h"

safuResultE_t elosPluginManagerInitialize(elosPluginManager_t *pluginManager, elosPluginManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (param->config == NULL) {
        safuLogErr("Invalid fields in parameter struct");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INVALID) {
        safuLogErr("The given pluginManager struct is not in state 'INVALID'");
    } else {
        result = elosPluginControlVectorInitialize(&pluginManager->pluginVector, PLUGINMANAGER_PLUGINVECTOR_INIT_SIZE);
        if (result < 0) {
            safuLogErr("elosPluginControlVectorInitialize failed");
        } else {
            pluginManager->config = param->config;
            pluginManager->state = PLUGINMANAGER_STATE_INITIALIZED;
            pluginManager->nextId = 1;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginManagerDeleteMembers(elosPluginManager_t *pluginManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (pluginManager != NULL) {
        result = elosPluginControlVectorDeleteMembers(&pluginManager->pluginVector);
    }

    return result;
}
