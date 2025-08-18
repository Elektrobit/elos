// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <pthread.h>
#include <safu/log.h>
#include <safu/result.h>
#include <stdlib.h>

#include "dlt_hv/scanner.h"
#include "dlt_scanner.h"

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            plugin->data = NULL;
            result = elosDltScannerInit(plugin);
            if (result == SAFU_RESULT_OK) {
                safuLogDebugF("Scanner DLT Plugin '%s' has been loaded", plugin->config->key);
            } else {
                safuLogErrF("Failed to load DLT Plugin '%s'", plugin->config->key);
            }
        }
    }

    return result;
}

static void *_scannerLoop(void *param) {
    return elosDltScannerLoop((elosDltScanner_t *)param);
}

static safuResultE_t _pluginRunLoops(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;
    result = elosDltScannerOpenBuffer(dlt);
    if (result == SAFU_RESULT_OK) {
        int threadRes = pthread_create(&dlt->scannerThread, NULL, _scannerLoop, (void *)dlt);
        if (threadRes != 0) {
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        result = elosDltPublisherLoop(plugin);
    }
    safuLogDebug("Joining scanner thread");
    pthread_join(dlt->scannerThread, NULL);
    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Scanner DLT Plugin '%s' has been started", plugin->config->key);

        result = elosPluginReportAsStarted(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginReportAsStarted failed");
        } else {
            result = _pluginRunLoops(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErrF("Scanner loop failed for %s", plugin->config->key);
            }
            result = elosPluginStopTriggerWait(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginStopTriggerWait failed");
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        if (plugin->data != NULL) {
            safuLogDebugF("Stopping Scanner DLT Plugin '%s'", plugin->config->key);
            elosDltScanner_t *dlt = plugin->data;
            result = elosDltScannerStopLoops(dlt);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("stoping scanner loop failed");
            }
        }
        safuResultE_t res = elosPluginStopTriggerWrite(plugin);
        if (res == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
            result = res;
        }
    }

    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Scanner DLT Plugin '%s'", plugin->config->key);
        result = elosDltScannerFree(plugin);
        free(plugin->data);
    }

    return result;
}

elosPluginConfig_t elosPluginConfig = {
    .type = PLUGIN_TYPE_SCANNER,
    .load = _pluginLoad,
    .unload = _pluginUnload,
    .start = _pluginStart,
    .stop = _pluginStop,
};
