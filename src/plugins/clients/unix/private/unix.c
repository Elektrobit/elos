// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/connectionmanager.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "unix_clientauthorization/clientauthorization.h"
#include "unix_config/config.h"

safuResultE_t _initializeListener(elosConnectionManager_t *connectionManager, elosPlugin_t const *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr *addr = (struct sockaddr *)&connectionManager->addr;
    struct sockaddr_un *unixAddr = (struct sockaddr_un *)addr;
    socklen_t addrLen = sizeof(struct sockaddr_un);
    int retVal = 0;

    result = elosUnixConfigGetSocketAddress(plugin, addr);

    if (result == SAFU_RESULT_OK) {
        connectionManager->fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (connectionManager->fd == -1) {
            safuLogErrErrnoValue("Socket creation failed", errno);
            result = SAFU_RESULT_FAILED;
        } else {
            int reuse = 1;
            retVal = setsockopt(connectionManager->fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
            if (retVal != 0) {
                safuLogErrErrnoValue("setsockopt SO_REUSEADDR failed", errno);
                result = SAFU_RESULT_FAILED;
            } else {
                retVal = bind(connectionManager->fd, addr, addrLen);
                if (retVal != 0) {
                    safuLogErrErrnoValue("Bind failed", errno);
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    retVal = chmod(unixAddr->sun_path, S_IRWXU | S_IRWXG | S_IRWXO);
    if (retVal != 0) {
        safuLogErrErrnoValue("chmod failed", errno);
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("listen on UNIX socket: %s", unixAddr->sun_path);
    } else {
        safuLogErrF("Failed to listening on UNIX socket: %s", unixAddr->sun_path);
    }

    return result;
}

static void _initConnectionManagerHelperApi(elosConnectionManager_t *connectionManager) {
    connectionManager->initializeListener = _initializeListener;
    connectionManager->getConnectionLimit = elosUnixConfigGetConnectionLimit;
    connectionManager->authorizationInitialize = elosUnixClientAuthorizationInitialize;
    connectionManager->authorizationIsValid = elosUnixClientAuthorizationIsTrustedConnection;
    connectionManager->authorizationDelete = elosUnixClientAuthorizationDelete;
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            elosConnectionManager_t *connectionManager = safuAllocMem(NULL, sizeof(elosConnectionManager_t));
            memset(connectionManager, 0 , sizeof(elosConnectionManager_t));
            if (connectionManager == NULL) {
                safuLogErr("Failed to allocate memory");
            } else {
                safuLogDebug("Start client manager");
                _initConnectionManagerHelperApi(connectionManager);
                result = elosConnectionManagerInitialize(connectionManager, plugin);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosConnectionManagerInitialize failed");
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
