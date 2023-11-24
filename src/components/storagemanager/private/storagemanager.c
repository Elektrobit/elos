// SPDX-License-Identifier: MIT
#include "elos/storagemanager/storagemanager.h"

#include <safu/common.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/config/config.h"
#include "elos/eventfilter/vector.h"
#include "elos/storagemanager/PluginFilterLoader.h"
#include "elos/storagemanager/StorageBackend.h"
#include "elos/storagemanager/vector.h"
#include "safu/log.h"

safuResultE_t elosStorageManagerInitialize(elosStorageManager_t *storageManager,
                                           elosStorageManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((storageManager == NULL) || (param == NULL)) {
        safuLogErr("Called elosStorageManagerInitialize with NULL-parameter");
    } else if ((param->config == NULL) || (param->pluginManager == NULL)) {
        safuLogErr("Parameter struct passed to elosStorageManagerInitialize contains NULL-pointers");
    } else {
        samconfConfigStatusE_t status;
        int retVal = 0;

        storageManager->pluginManager = param->pluginManager;
        retVal = safuVecCreate(&storageManager->pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            storageManager->searchPath = elosConfigGetElosdBackendPath(param->config);
            status = samconfConfigGet(param->config, ELOS_CONFIG_EVENTLOGGING, &storageManager->config);
            if (status != SAMCONF_CONFIG_OK) {
                safuLogErr("Loading storageManager config failed");
            } else {
                result = SAFU_RESULT_OK;
            }
        }
    }
    return result;
}

static int _setupBackend(void const *element, UNUSED void const *data) {
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    elosStorageManager_t *storageManager = (elosStorageManager_t *)data;
    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->context.data;

    elosPluginFilterLoaderLoad(plugin);
    if (backend != NULL) {
        int result = elosStorageBackendPtrVectorPush(&storageManager->backends, backend);
        if (result < 0) {
            safuLogErrF("elosStorageBackendPtrVectorPush failed for %s", backend->name);
        }
    } else {
        safuLogErrF("Load storage plugin without assigned storage backend (plugin id: %d)", plugin->context.id);
    }

    return 0;
}

safuResultE_t elosStorageManagerStart(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (storageManager == NULL) {
        safuLogErr("Called elosStorageManagerAdd with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        elosPluginTypeE_t type = PLUGIN_TYPE_STORAGEBACKEND;

        result = elosPluginManagerLoad(storageManager->pluginManager, type, storageManager->config,
                                       storageManager->searchPath, &storageManager->pluginControlPtrVector);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosPluginManagerStart executed with errors");
        } else {
            size_t pluginCount = safuVecElements(&storageManager->pluginControlPtrVector);
            result = elosStorageBackendPtrVectorInitialize(&storageManager->backends, pluginCount);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosStorageBackendPtrVectorInitialize failed");
            } else {
                safuVecIterate(&storageManager->pluginControlPtrVector, _setupBackend, storageManager);
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

safuResultE_t elosStorageManagerStop(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (storageManager != NULL) {
        if (storageManager->pluginManager != NULL) {
            safuVecIterate(&storageManager->pluginControlPtrVector, _unloadFilter, NULL);
            elosStorageBackendPtrVectorDeleteMembers(&storageManager->backends);
            result = elosPluginManagerUnload(storageManager->pluginManager, &storageManager->pluginControlPtrVector);
        }
    }

    return result;
}

safuResultE_t elosStorageManagerDeleteMembers(elosStorageManager_t *storageManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (storageManager != NULL && storageManager->pluginManager != NULL) {
        int retVal;

        retVal = safuVecFree(&storageManager->pluginControlPtrVector);
        if (retVal == 0) {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
