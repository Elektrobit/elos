// SPDX-License-Identifier: MIT

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

safuResultE_t elosPluginLoad(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
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

safuResultE_t elosPluginStart(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        eventfd_t efdVal = 0;
        int retVal;

        result = elosSqlBackendStart(plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosSqlBackendStart failed");
        }

        retVal = eventfd_write(plugin->worker.sync, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (worker.sync) failed");
            result = SAFU_RESULT_FAILED;
        }

        retVal = eventfd_read(plugin->stop, &efdVal);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read (stop) failed");
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

        result = elosSqlBackendShutdown(plugin->data);
        plugin->data = NULL;
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosSqlBackendShutdown failed (likely resulting in a memory leak)");
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
