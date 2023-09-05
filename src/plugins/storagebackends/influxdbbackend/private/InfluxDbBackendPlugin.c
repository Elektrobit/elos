// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <ctype.h>
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
#include "elos/plugin/types.h"

safuResultE_t elosPluginLoad(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
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

safuResultE_t elosPluginStart(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        eventfd_t efdVal = 0;
        int retVal;

        result = elosInfluxDbBackendStart(plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosInfluxDbBackendStart failed");
        }

        retVal = eventfd_write(plugin->worker.sync, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (worker.sync) failed");
            result = SAFU_RESULT_FAILED;
        }

        retVal = eventfd_read(plugin->stop, &efdVal);
        if (retVal < 0) {
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosPluginStop(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        int retVal;

        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        result = elosInfluxDbBackendShutdown(plugin->data);
        plugin->data = NULL;
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosInfluxDbBackendShutdown failed (likely resulting in a memory leak)");
        }

        retVal = eventfd_write(plugin->stop, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (stop) failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosPluginUnload(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
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
