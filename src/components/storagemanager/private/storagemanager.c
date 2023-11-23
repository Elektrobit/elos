// SPDX-License-Identifier: MIT
#include "elos/storagemanager/storagemanager.h"

#include <safu/common.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/config/config.h"
#include "safu/log.h"
#include "elos/storagemanager/PluginFilterLoader.h"

safuResultE_t elosStorageManagerInitialize(elosStorageManager_t *storageManager,
                                           elosStorageManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((storageManager == NULL) || (param == NULL)) {
        safuLogErr("Called elosStorageManagerInitialize with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else if ((param->config == NULL) || (param->pluginManager == NULL)) {
        safuLogErr("Parameter struct passed to elosStorageManagerInitialize contains NULL-pointers");
        result = SAFU_RESULT_FAILED;
    } else {
        samconfConfigStatusE_t status;
        int retVal = 0;

        storageManager->pluginManager = param->pluginManager;
        retVal = safuVecCreate(&storageManager->pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
            result = SAFU_RESULT_FAILED;
        } else {
            storageManager->searchPath = elosConfigGetElosdBackendPath(param->config);
            if (storageManager->searchPath == NULL) {
                safuLogErr("elosConfigGetElosdBackendPath failed");
                result = SAFU_RESULT_FAILED;
            } else {
                status = samconfConfigGet(param->config, ELOS_CONFIG_EVENTLOGGING, &storageManager->config);
                if (status != SAMCONF_CONFIG_OK) {
                    safuLogErr("Loading storageManager config failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }
    return result;
}

safuResultE_t elosStorageManagerStart(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (storageManager == NULL) {
        safuLogErr("Called elosStorageManagerAdd with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        elosPluginTypeE_t type = PLUGIN_TYPE_STORAGEBACKEND;

        result = elosPluginManagerLoad(storageManager->pluginManager, type, storageManager->config,
                                       storageManager->searchPath, &storageManager->pluginControlPtrVector);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosPluginManagerStart executed with errors");
        }
    }
    return result;
}

safuResultE_t elosStorageManagerStop(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (storageManager != NULL) {
        if (storageManager->pluginManager != NULL) {
            safuResultE_t funcResult;

            funcResult =
                elosPluginManagerUnload(storageManager->pluginManager, &storageManager->pluginControlPtrVector);
            if (funcResult != SAFU_RESULT_OK) {
                result = funcResult;
            }
        }
    }

    return result;
}

safuResultE_t elosStorageManagerDeleteMembers(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (storageManager != NULL) {
        int retVal;
        if (storageManager->pluginManager != NULL) {
            retVal = safuVecFree(&storageManager->pluginControlPtrVector);
            if (retVal < 0) {
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}
