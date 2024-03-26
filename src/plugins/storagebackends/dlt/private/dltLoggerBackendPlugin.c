// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "dltLoggerBackend.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/storagemanager/StorageBackend_types.h"

static safuResultE_t _backendStart(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == NULL || backend->backendData == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosDltLoggerBackendStart(backend->backendData);
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _backendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == NULL) || (event == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        result = elosDltLoggerBackendPersist(backend->backendData, event);
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _backendFindEvent(elosStorageBackend_t *backend, elosRpnFilter_t *filter, safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == NULL) || (filter == NULL) || (events == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebug("Not implemented");
        elosRpnFilterResultE_t filterResult;

        elosEvent_t event = {0};
        filterResult = elosEventFilterExecute(filter, NULL, &event);
        if (filterResult == RPNFILTER_RESULT_MATCH || filterResult == RPNFILTER_RESULT_NO_MATCH) {
            result = SAFU_RESULT_OK;
        } else {
            safuLogErr("elosEventFilterExecute failed");
            safuLogDebugF("Free Event: RPN filter result is: %d", filterResult);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

static safuResultE_t _backendShutdown(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosDltLoggerBackendStop(backend->backendData);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed to stop dlt backend");
        }

        result = elosDltLoggerBackendDeleteMembers(backend->backendData);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosDltLoggerBackendDeleteMembers failed");
        }
    }

    return result;
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        elosStorageBackend_t *newBackend;

        newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
        if (newBackend == NULL) {
            safuLogErr("Memory allocation failed");
        } else if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            elosStorageBackend_t const backendValues = {
                .name = plugin->config->key,
                .backendData = NULL,
                .start = &_backendStart,
                .persist = &_backendPersist,
                .findEvent = &_backendFindEvent,
                .shutdown = &_backendShutdown,
            };

            memcpy(newBackend, &backendValues, sizeof(elosStorageBackend_t));
            plugin->data = newBackend;

            elosDltLoggerBackendParam_t param = {0};
            samconfConfigStatusE_t retVal =
                samconfConfigGetString(plugin->config, "/Config/Connection", &param.connectionString);
            if (retVal == SAMCONF_CONFIG_NOT_FOUND) {
                safuLogErrF("Missing option '/Config/Connection' for '%s'", plugin->config->key);
            }

            retVal = samconfConfigGetString(plugin->config, "/Config/EcuId", &param.ecuId);
            if (retVal == SAMCONF_CONFIG_NOT_FOUND) {
                safuLogErrF("Missing option '/Config/EcuId' for '%s'", plugin->config->key);
            }

            retVal = samconfConfigGetString(plugin->config, "/Config/AppId", &param.appId);
            if (retVal == SAMCONF_CONFIG_NOT_FOUND) {
                safuLogErrF("Missing option '/Config/AppId' for '%s'", plugin->config->key);
            }

            result = elosDltLoggerBackendNew((elosDltLoggerBackend_t **)&newBackend->backendData, &param);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Failed to initialise '%s' DLT-backend failed", plugin->config->key);
            } else {
                safuLogDebugF("Plugin '%s' has been loaded", plugin->config->key);
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Plugin '%s' has been started", plugin->config->key);

        result = elosPluginReportAsStarted(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginReportAsStarted failed");
        } else {
            _backendStart(plugin->data);
            result = elosPluginStopTriggerWait(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginStopTriggerWait failed");
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        _backendShutdown(plugin->data);
        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        elosStorageBackend_t *backend = plugin->data;
        elosDltLoggerBackendDelete(backend->backendData);
        free(backend);
        result = SAFU_RESULT_OK;
    }

    return result;
}

elosPluginConfig_t elosPluginConfig = {
    .type = PLUGIN_TYPE_STORAGEBACKEND,
    .load = _pluginLoad,
    .unload = _pluginUnload,
    .start = _pluginStart,
    .stop = _pluginStop,
};
