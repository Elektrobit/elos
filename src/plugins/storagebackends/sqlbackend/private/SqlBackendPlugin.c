// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <samconf/samconf.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "SqlBackend.h"
#include "elos/plugincontrol/types.h"

#ifndef SQLITE_DEFAULT_CONNECTION
#define SQLITE_DEFAULT_CONNECTION "/var/db/elosd.sqlite"
#endif

static inline const char *_getConnectionString() {
    return safuGetEnvOr("ELOS_STORAGE_BACKEND_SQL_CONNECTION", SQLITE_DEFAULT_CONNECTION);
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosSqlBackendNew((elosStorageBackend_t **)&plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosSqlBackendNew failed");
        }
    }

    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->data;
    elosSqlBackend_t *sqlBackend = (elosSqlBackend_t *)backend->backendData;
    samconfConfigStatusE_t retVal =
        samconfConfigGetString(plugin->config, "Config/ConnectionPath", (const char **)&sqlBackend->connectionString);
    if (retVal != SAMCONF_CONFIG_OK) {
        sqlBackend->connectionString = _getConnectionString();
    }

    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        result = elosSqlBackendStart(plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosSqlBackendStart failed");
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
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        result = elosSqlBackendShutdown(plugin->data);
        plugin->data = NULL;
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosSqlBackendShutdown failed (likely resulting in a memory leak)");
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

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
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
