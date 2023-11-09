// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <ctype.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <fcntl.h>
#include <glob.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/time.h>
#include <samconf/samconf.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "InfluxDb.h"

safuResultE_t elosPluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosInfluxDbBackendNew((elosStorageBackend_t **)&plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosInfluxDbBackendNew failed");
        }
    }

    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->data;
    elosInfluxDbBackend_t *influxBackend = (elosInfluxDbBackend_t *)backend->backendData;
    influxBackend->host = "localhost:8086";
    influxBackend->user = "root";
    influxBackend->pw = "emlixemlix";
    influxBackend->orgId = "emlix";
    influxBackend->db = "elosd";

    return result;
}

safuResultE_t elosPluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    }

    if (result != SAFU_RESULT_FAILED) {
        result = elosInfluxDbBackendStart(plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosInfluxDbBackendStart failed");
        }
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

    return result;
}

safuResultE_t elosPluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        result = elosInfluxDbBackendShutdown(plugin->data);
        plugin->data = NULL;
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosInfluxDbBackendShutdown failed (likely resulting in a memory leak)");
        }

        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

safuResultE_t elosPluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else if ((plugin->state != PLUGIN_STATE_LOADED) && (plugin->state != PLUGIN_STATE_STOPPED)) {
        safuLogErrF("Plugin is not in state 'LOADED' or 'STOPPED' (state=%d)", plugin->state);
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        result = SAFU_RESULT_OK;
    }

    return result;
}
