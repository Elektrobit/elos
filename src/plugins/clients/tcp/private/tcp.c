// SPDX-License-Identifier: MIT

#include <elos/connectionmanager/connectionmanager.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            elosConnectionManager_t *connectionManager = safuAllocMem(NULL, sizeof(elosConnectionManager_t));
            if (connectionManager == NULL) {
                safuLogErr("Failed to allocate memory");
            } else {
                safuLogDebug("Start client manager");
                elosConnectionManagerParam_t cmParams = {
                    .config = (samconfConfig_t *)plugin->config,
                    .plugin = plugin,
                };
                result = elosConnectionManagerInitialize(connectionManager, &cmParams);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosConnectionManagerInitialize");
                } else {
                    plugin->data = connectionManager;
                    safuLogDebugF("Plugin '%s' has been loaded", plugin->config->key);
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosConnectionManager_t *connectionManager = plugin->data;

    if (plugin == NULL || connectionManager == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebug("Start connection manager");
        result = elosConnectionManagerStart(connectionManager);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosConnectionManagerStart");
        } else {
            safuLogDebugF("Plugin '%s' has been started", plugin->config->key);
            result = elosPluginReportAsStarted(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginReportAsStarted failed");
            } else {
                result = elosPluginStopTriggerWait(plugin);
                if (result == SAFU_RESULT_FAILED) {
                    safuLogErr("elosPluginStopTriggerWait failed");
                }
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosConnectionManager_t *connectionManager = plugin->data;

    if (plugin == NULL || connectionManager == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        if (elosConnectionManagerStop(connectionManager) != SAFU_RESULT_OK) {
            safuLogErr("Stoping connection manager failed!");
            result = EXIT_FAILURE;
        }
        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosConnectionManager_t *connectionManager = plugin->data;

    if (plugin == NULL || connectionManager == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        if (elosConnectionManagerDeleteMembers(connectionManager) != SAFU_RESULT_OK) {
            safuLogErr("Deleting connection manager failed!");
        }
        free(plugin->data);
        result = SAFU_RESULT_OK;
    }

    return result;
}

elosPluginConfig_t elosPluginConfig = {
    .type = PLUGIN_TYPE_CLIENTCONNECTION,
    .load = _pluginLoad,
    .unload = _pluginUnload,
    .start = _pluginStart,
    .stop = _pluginStop,
};
