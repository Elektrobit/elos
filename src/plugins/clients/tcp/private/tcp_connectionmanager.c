// SPDX-License-Identifier: MIT

#include "tcp_connectionmanager_private.h"

safuResultE_t _initializeListener(elosConnectionManager_t *connectionManager, elosPlugin_t const *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr *addr = (struct sockaddr *)&connectionManager->addr;
    socklen_t addrLen = 0;
    int retVal = 0;
    char const *interface = elosTcpConfigGetInterface(plugin);
    int const port = elosTcpConfigGetPort(plugin);

    result = elosTcpConfigGetSocketAddress(plugin, addr);
    addrLen = sizeof(struct sockaddr_in);

    if (result == SAFU_RESULT_OK) {
        connectionManager->fd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("listen on: %s:%d", interface, port);
    } else {
        safuLogErrF("Failed to listening on %s:%d", interface, port);
    }

    return result;
}

safuResultE_t elosTcpConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    *connectionManager = safuAllocMem(NULL, sizeof(elosConnectionManager_t));

    if (*connectionManager == NULL) {
        safuLogErr("Failed to allocate memory");
    } else {
        safuLogDebug("Start client manager");
        memset(*connectionManager, 0, sizeof(elosConnectionManager_t));

        (*connectionManager)->initializeListener = _initializeListener;
        (*connectionManager)->getConnectionLimit = elosTcpConfigGetConnectionLimit;
        (*connectionManager)->authorizationInitialize = elosTcpClientAuthorizationInitialize;
        (*connectionManager)->authorizationIsValid = elosTcpClientAuthorizationIsTrustedConnection;
        (*connectionManager)->authorizationDelete = elosTcpClientAuthorizationDelete;

        result = elosConnectionManagerInitialize(*connectionManager, plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosConnectionManagerInitialize failed");
        }
    }

    return result;
}