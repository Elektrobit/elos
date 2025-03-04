// SPDX-License-Identifier: MIT

#include "unix_connectionmanager_private.h"

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

safuResultE_t elosUnixConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    *connectionManager = safuAllocMem(NULL, sizeof(elosConnectionManager_t));

    if (*connectionManager == NULL) {
        safuLogErr("Failed to allocate memory");
    } else {
        safuLogDebug("Start client manager");
        memset(*connectionManager, 0, sizeof(elosConnectionManager_t));

        (*connectionManager)->initializeListener = _initializeListener;
        (*connectionManager)->getConnectionLimit = elosUnixConfigGetConnectionLimit;
        (*connectionManager)->authorizationInitialize = elosUnixClientAuthorizationInitialize;
        (*connectionManager)->authorizationIsValid = elosUnixClientAuthorizationIsTrustedConnection;
        (*connectionManager)->authorizationDelete = elosUnixClientAuthorizationDelete;

        result = elosConnectionManagerInitialize(*connectionManager, plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosConnectionManagerInitialize failed");
        }
    }

    return result;
}