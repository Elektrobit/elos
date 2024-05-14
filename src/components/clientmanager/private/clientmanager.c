// SPDX-License-Identifier: MIT
#include "elos/clientmanager/clientmanager.h"

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>

#include "elos/config/config.h"
#include "safu/log.h"

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientManager, elosClientManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((clientManager == NULL) || (param == NULL)) {
        safuLogErr("Called elosClientManagerInitialize with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else if ((param->config == NULL) || (param->pluginManager == NULL)) {
        safuLogErr("Parameter struct passed to elosClientManagerInitialize contains NULL-pointers");
        result = SAFU_RESULT_FAILED;
    } else {
        samconfConfigStatusE_t status;
        int retVal = 0;

        clientManager->pluginManager = param->pluginManager;
        retVal = safuVecCreate(&clientManager->pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
            result = SAFU_RESULT_FAILED;
        } else {
            clientManager->searchPath = elosConfigGetElosdClientPath(param->config);
            status = samconfConfigGet(param->config, ELOS_CONFIG_CLIENTINPUTS, &clientManager->config);
            switch (status) {
                case SAMCONF_CONFIG_NOT_FOUND:
                    clientManager->config = NULL;
                    __attribute__((fallthrough));
                case SAMCONF_CONFIG_OK:
                    break;
                default:
                    safuLogErr("Error in ClientInputs config");
                    result = SAFU_RESULT_FAILED;
            }
        }
    }
    return result;
}

safuResultE_t elosClientManagerStart(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientManager == NULL) {
        safuLogErr("Called elosClientManagerAdd with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else if (clientManager->config == NULL) {
        safuLogDebug("No ClientInputs configured");
        result = SAFU_RESULT_OK;
    } else {
        elosPluginTypeE_t type = PLUGIN_TYPE_CLIENTCONNECTION;

        result = elosPluginManagerLoad(clientManager->pluginManager, type, clientManager->config,
                                       clientManager->searchPath, &clientManager->pluginControlPtrVector);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosPluginManagerLoad executed with errors");

        } else {
            safuLogInfoF("ClientManager loaded %d client plugins", safuVecElements(&clientManager->pluginControlPtrVector));
        }
    }
    return result;
}

safuResultE_t elosClientManagerStop(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientManager != NULL) {
        if (clientManager->pluginManager != NULL) {
            result = elosPluginManagerUnload(clientManager->pluginManager, &clientManager->pluginControlPtrVector);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("elosPluginManagerStop executed with errors");
            }
        }
    }

    return result;
}

safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (clientManager != NULL) {
        int retVal;
        if (clientManager->pluginManager != NULL) {
            retVal = safuVecFree(&clientManager->pluginControlPtrVector);
            if (retVal == 0) {
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}
