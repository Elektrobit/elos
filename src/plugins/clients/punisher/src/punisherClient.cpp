// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <stdlib.h>

#include "punisher.h"

Punisher *elosPunisher;
elosSubscriber *elosSbscriber;

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            result = elosPluginCreateSubscriber(plugin, &elosSbscriber);
            if (result == SAFU_RESULT_OK) {
                elosPunisher = new (std::nothrow) Punisher(plugin, elosSbscriber);
            } else {
                safuLogErr("Could not create subscriber");
            }

            if (elosPunisher == nullptr) {
                safuLogErr("Allocating punisher failed!");
            } else {
                plugin->data = NULL;

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

        if (elosPunisher != nullptr) {
            elosPunisher->runEventHandlingThread();
        }

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

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        if (elosPunisher != nullptr) {
            elosPunisher->stopEventHandlingThread();
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

    if (plugin == NULL || plugin->config == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        free(plugin->data);
        delete elosPunisher;
        result = elosPluginDeleteSubscriber(plugin, elosSbscriber);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("Failed to delete subscriber");
        }
    }

    return result;
}

elosPluginConfig_t elosPluginConfig = {
    PLUGIN_TYPE_CLIENTCONNECTION, _pluginLoad, _pluginUnload, _pluginStart, _pluginStop,
};
