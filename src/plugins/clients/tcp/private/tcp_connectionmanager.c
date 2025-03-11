// SPDX-License-Identifier: MIT

#include <netinet/in.h>
#include <stdlib.h>
#include <tcp_clientauthorization/clientauthorization.h>
#include <tcp_config/config.h>
#include <unistd.h>

#include "tcp_clientconnection_private.h"
#include "tcp_connectionmanager_private.h"

safuResultE_t _initializeListener(elosConnectionManager_t *connectionManager, elosPlugin_t const *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr_in *addr = safuAllocMem(NULL, sizeof(struct sockaddr_in));
    socklen_t addrLen = sizeof(struct sockaddr_in);
    int retVal = 0;
    char const *interface = elosTcpConfigGetInterface(plugin);
    int const port = elosTcpConfigGetPort(plugin);

    if (addr != NULL) {
        connectionManager->connectionManagerContext = (void *)addr;
        result = elosTcpConfigGetSocketAddress(plugin, addr);
    } else {
        safuLogErr("Failed to allocate memory");
    }

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
                retVal = bind(connectionManager->fd, (struct sockaddr *)addr, addrLen);
                if (retVal != 0) {
                    safuLogErrErrnoValue("Bind failed", errno);
                    result = SAFU_RESULT_FAILED;
                } else {
                    retVal = listen(connectionManager->fd, ELOS_CONNECTIONMANAGER_LISTEN_QUEUE_LENGTH);
                    if (retVal != 0) {
                        safuLogErrErrnoValue("listen failed", retVal);
                        result = SAFU_RESULT_FAILED;
                    }
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

safuResultE_t _accept(elosConnectionManager_t *connectionManager, elosClientConnection_t *connection) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in *addr = (struct sockaddr_in *)connection->clientConnectionContext;

    if (addr != NULL) {
        connection->fd = accept(connectionManager->fd, (struct sockaddr *)addr, &addrLen);
        if (connection->fd == -1) {
            if (errno == EINTR) {
                result = SAFU_RESULT_OK;
            } else {
                safuLogErrErrno("accept failed!");
                result = SAFU_RESULT_FAILED;
            }
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t _closeListener(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal = 0;

    if (connectionManager->fd != -1) {
        retVal = close(connectionManager->fd);
        if (retVal != 0) {
            safuLogWarnErrnoValue("Closing listenFd failed (possible memory leak)", retVal);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t _deleteListener(elosConnectionManager_t *connectionManager) {
    free(connectionManager->connectionManagerContext);

    return SAFU_RESULT_OK;
}

safuResultE_t _authorize(elosConnectionManager_t *connectionManager, elosClientConnection_t *connection) {
    connection->isTrusted = elosTcpClientAuthorizationIsTrustedConnection(
        &connectionManager->clientAuth, (struct sockaddr const *)connection->clientConnectionContext);
    return SAFU_RESULT_OK;
}

safuResultE_t _setConnectionHandlers(elosClientConnection_t *connection) {
    connection->initializeConnection = elosTcpClientConnectionInitialize;
    connection->deleteConnection = elosTcpClientConnectionDelete;
    connection->closeConnection = elosTcpClientConnectionClose;
    return SAFU_RESULT_OK;
}

safuResultE_t elosTcpConnectionManagerDelete(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosConnectionManagerDeleteMembers(connectionManager);
    free(plugin->data);

    return result;
}

safuResultE_t elosTcpConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    *connectionManager = safuAllocMem(NULL, sizeof(elosConnectionManager_t));

    if (*connectionManager == NULL) {
        safuLogErr("Failed to allocate memory");
    } else {
        memset(*connectionManager, 0, sizeof(elosConnectionManager_t));

        (*connectionManager)->initializeListener = _initializeListener;
        (*connectionManager)->closeListener = _closeListener;
        (*connectionManager)->accept = _accept;
        (*connectionManager)->deleteListener = _deleteListener;
        (*connectionManager)->setConnectionHandlers = _setConnectionHandlers;
        (*connectionManager)->getConnectionLimit = elosTcpConfigGetConnectionLimit;
        (*connectionManager)->authorizationInitialize = elosTcpClientAuthorizationInitialize;
        (*connectionManager)->authorize = _authorize;
        (*connectionManager)->authorizationDelete = elosTcpClientAuthorizationDelete;

        result = elosConnectionManagerInitialize(*connectionManager, plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosConnectionManagerInitialize failed");
            if (elosTcpConnectionManagerDelete(*connectionManager, plugin) != SAFU_RESULT_OK) {
                safuLogErr("Deleting connection manager failed!");
            }
        }
    }

    return result;
}