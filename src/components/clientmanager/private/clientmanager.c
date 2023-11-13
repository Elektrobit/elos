// SPDX-License-Identifier: MIT
#include "elos/clientmanager/clientmanager.h"

#include <safu/common.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/config/config.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/vector.h"
#include "elos/eventlogging/PluginFilterLoader.h"
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
        } else {
            clientManager->searchPath = elosConfigGetElosdClientPath(param->config);
            if (clientManager->searchPath == NULL) {
                safuLogErr("elosConfigGetElosdClientPath failed");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static int _loadFilter(void const *element, UNUSED void const *data) {
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    elosPluginFilterLoaderLoad(plugin);
    return 0;
}

safuResultE_t elosClientManagerLoad(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientManager == NULL) {
        safuLogErr("Called elosClientManagerAdd with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        char const *searchPath;
        searchPath = elosConfigGetElosdBackendPath(clientManager->config);
        if (searchPath == NULL) {
            safuLogErr("elosConfigGetElosdBackendPath failed");
            result = SAFU_RESULT_FAILED;
        } else {
            elosPluginTypeE_t type = PLUGIN_TYPE_CLIENTCONNECTION;

            result = elosPluginManagerLoad(clientManager->pluginManager, type, clientManager->config, searchPath,
                                           &clientManager->pluginControlPtrVector);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("elosPluginManagerLoad executed with errors");
            } else {
                safuVecIterate(&clientManager->pluginControlPtrVector, _loadFilter, NULL);
            }
        }
    }
    return result;
}

static int _unloadFilter(void const *element, UNUSED void const *data) {
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->context.data;
    elosEventFilterVectorDeleteMembers(&backend->filter);
    return 0;
}

safuResultE_t elosClientManagerShutdown(elosClientManager_t *clientManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientManager != NULL) {
        int retVal;

        if (clientManager->pluginManager != NULL) {
            safuResultE_t funcResult;

            safuVecIterate(&clientManager->pluginControlPtrVector, _unloadFilter, NULL);

            funcResult = elosPluginManagerUnload(clientManager->pluginManager, &clientManager->pluginControlPtrVector);
            if (funcResult != SAFU_RESULT_OK) {
                result = funcResult;
            }

            retVal = safuVecFree(&clientManager->pluginControlPtrVector);
            if (retVal < 0) {
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}
