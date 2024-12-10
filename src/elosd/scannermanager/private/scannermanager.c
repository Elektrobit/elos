// SPDX-License-Identifier: MIT
#include "elos/scannermanager/scannermanager.h"

#include <safu/common.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/config/config.h"
#include "safu/log.h"

safuResultE_t elosScannerManagerInitialize(elosScannerManager_t *scannerManager,
                                           elosScannerManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((scannerManager == NULL) || (param == NULL)) {
        safuLogErr("Called elosScannerManagerInitialize with NULL-parameter");
    } else if ((param->config == NULL) || (param->pluginManager == NULL)) {
        safuLogErr("Parameter struct passed to elosScannerManagerInitialize contains NULL-pointers");
    } else {
        samconfConfigStatusE_t status;
        int retVal = 0;

        scannerManager->pluginManager = param->pluginManager;
        retVal = safuVecCreate(&scannerManager->pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
            result = SAFU_RESULT_FAILED;
        } else {
            scannerManager->searchPath = elosConfigGetElosdScannerPath(param->config);
            status = samconfConfigGet(param->config, ELOS_CONFIG_SCANNER, &scannerManager->config);
            if (status == SAMCONF_CONFIG_NOT_FOUND) {
                scannerManager->config = NULL;
                result = SAFU_RESULT_OK;
            } else if (status == SAMCONF_CONFIG_OK && scannerManager->config->type == SAMCONF_CONFIG_VALUE_OBJECT) {
                result = SAFU_RESULT_OK;
            } else {
                safuLogErr("Error in Scanner config");
                result = SAFU_RESULT_FAILED;
            }
        }
    }
    return result;
}

safuResultE_t elosScannerManagerStart(elosScannerManager_t *scannerManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (scannerManager == NULL) {
        safuLogErr("Called elosScannerManagerAdd with NULL-parameter");
    } else if (scannerManager->config == NULL) {
        safuLogDebug("No Scanner configured");
        result = SAFU_RESULT_OK;
    } else {
        elosPluginTypeE_t type = PLUGIN_TYPE_SCANNER;

        result = elosPluginManagerLoad(scannerManager->pluginManager, type, scannerManager->config,
                                       scannerManager->searchPath, &scannerManager->pluginControlPtrVector);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosPluginManagerStart executed with errors");
        } else {
            safuLogInfoF("ScannerManager loaded %d scanner plugins",
                         safuVecElements(&scannerManager->pluginControlPtrVector));
        }
    }
    return result;
}

safuResultE_t elosScannerManagerStop(elosScannerManager_t *scannerManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (scannerManager != NULL && scannerManager->pluginManager != NULL) {
        result = elosPluginManagerUnload(scannerManager->pluginManager, &scannerManager->pluginControlPtrVector);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosPluginManagerStop executed with errors");
        }
    }
    return result;
}

safuResultE_t elosScannerManagerDeleteMembers(elosScannerManager_t *scannerManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (scannerManager != NULL && scannerManager->pluginManager != NULL) {
        int retVal;

        retVal = safuVecFree(&scannerManager->pluginControlPtrVector);
        if (retVal == 0) {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
