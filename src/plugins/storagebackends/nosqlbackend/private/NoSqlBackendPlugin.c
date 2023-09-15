// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <safu/log.h>
#include <samconf/samconf.h>
#include <sys/eventfd.h>

#include "NoSqlBackend.h"
#include "elos/plugin/types.h"

static inline char *_upperString(const char *src) {
    size_t i;
    size_t srcLen = strlen(src);
    char *dest = safuAllocMem(NULL, sizeof(char) * (srcLen + 1));
    for (i = 0; i < srcLen; i++) {
        dest[i] = toupper(src[i]);
    }
    dest[srcLen] = '\0';
    return dest;
}

static inline const char *_getConnectionString(const char *backendName) {
    const char *prefix = "ELOS_STORAGE_BACKEND_"; 
    const char *postfix = "_CONNECTION";
    const size_t prefixLen = strlen(prefix);
    const size_t postfixLen = strlen(postfix);
    const size_t nameLen = strlen(backendName);
    char *envVar = safuAllocMem(NULL, sizeof(char) * (prefixLen + postfixLen + nameLen + 1));
    char *upperName = _upperString(backendName); 
    strcpy(envVar, prefix);
    strcpy(envVar + prefixLen, upperName); 
    strcpy(envVar + prefixLen + nameLen, postfix);
    const char *ret = safuGetEnvOr(envVar, "");
    free(envVar);
    free(upperName);
    return ret;
} 

safuResultE_t elosPluginLoad(void *pluginPtr) {
    elosPlugin_t *plugin = (elosPlugin_t *)pluginPtr;
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosNoSqlBackendNew((elosStorageBackend_t **)&plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosNoSqlBackendNew failed");
        }
    }

    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->data;
    elosNoSqlBackend_t *noSqlBackend = (elosNoSqlBackend_t *)backend->backendData;

    noSqlBackend->connectionString = (char *)_getConnectionString(plugin->config->key);
    if (strcmp(noSqlBackend->connectionString, "") == 0) {                     
        samconfConfigStatusE_t retVal = samconfConfigGetString(plugin->config, "Config/ConnectionString",
                                                               (const char **)&noSqlBackend->connectionString);
        if ((retVal != SAMCONF_CONFIG_OK) || (strcmp(noSqlBackend->connectionString, "") == 0)) {
            noSqlBackend->connectionString = CONNECTION_STRING;                 
        }                                                                       
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

        result = elosNoSqlBackendStart(plugin->data);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosNoSqlBackendStart failed");
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

        result = elosNoSqlBackendShutdown(plugin->data);
        plugin->data = NULL;
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("elosNoSqlBackendShutdown failed (likely resulting in a memory leak)");
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
