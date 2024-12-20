// SPDX-License-Identifier: MIT

#include "connectionmanager/connectionmanager.h"

#include <arpa/inet.h>
#include <elos/libelosplugin/types.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <stdlib.h>
#include <sys/eventfd.h>

#include "clientauthorizedprocesses/clientauthorizedprocesses.h"
#include "connectionmanager/clientblacklist.h"
#include "connectionmanager/clientconnection.h"
#include "connectionmanager_private.h"
#include "tcp_clientauthorization/clientauthorization.h"
#include "tcp_config/config.h"
#include "unix_config/config.h"

static safuResultE_t _initializeSharedData(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionSharedData_t *sharedData = &connectionManager->sharedData;
    int retVal;

    retVal = sem_init(&sharedData->connectionSemaphore, 0, ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT);
    if (retVal != 0) {
        safuLogErrErrnoValue("sem_init failed!", retVal);
    } else {
        sharedData->plugin = plugin;
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _initializeListener(elosConnectionManager_t *connectionManager, elosPlugin_t const *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct sockaddr *addr = (struct sockaddr *)&connectionManager->addr;
    socklen_t addrLen = 0;
    int retVal = 0;
    sa_family_t saFamily = connectionManager->saFamily;
    char const *interface = elosTcpConfigGetInterface(plugin);
    int const port = elosTcpConfigGetPort(plugin);

    switch (saFamily) {
        case AF_INET:
            result = elosTcpConfigGetSocketAddress(plugin, addr);
            addrLen = sizeof(struct sockaddr_in);
            break;
        case AF_UNIX:
            result = elosUnixConfigGetSocketAddress(plugin, addr);
            addrLen = sizeof(struct sockaddr_un);
            break;
        default:
            result = SAFU_RESULT_FAILED;
            break;
    }

    if (result == SAFU_RESULT_OK) {
        connectionManager->fd = socket(saFamily, SOCK_STREAM, 0);
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

    switch (saFamily) {
        case AF_INET:
            if (result == SAFU_RESULT_OK) {
                safuLogDebugF("listen on: %s:%d", interface, port);
            } else {
                safuLogErrF("Failed to listening on %s:%d", interface, port);
            }
            break;
        case AF_UNIX:
            if (result == SAFU_RESULT_OK) {
                struct sockaddr_un *unixAddr = (struct sockaddr_un *)addr;
                safuLogDebugF("listen on UNIX socket: %s", unixAddr->sun_path);
            } else {
                struct sockaddr_un *unixAddr = (struct sockaddr_un *)addr;
                safuLogErrF("Failed to listening on UNIX socket: %s", unixAddr->sun_path);
            }

            break;
        default:
            break;
    }

    return result;
}

static safuResultE_t _initializeConnections(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (int i = 0; i < ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT; i += 1) {
        elosClientConnection_t *connection = &connectionManager->connection[i];

        result = elosClientConnectionInitialize(connection, &connectionManager->sharedData);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Iniitialization of connection data structure [%d] failed", i);
            break;
        }
    }

    return result;
}

static safuResultE_t _initializeAuthorization(elosConnectionManager_t *connectionManager,
                                              samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosClientAuthorizationInitialize(&connectionManager->clientAuth);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosClientAuthorizationInitialize failed");
    }

    result = elosAuthorizedProcessInitialize(&connectionManager->clientAuth.authorizedProcessFilters, config);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("authorized process filter initialization failed");
    } else {
        safuLogDebug("authorized process filter initialization succeeded");
    }

    return result;
}

static safuResultE_t _initializeAddressFamily(elosConnectionManager_t *connectionManager, sa_family_t saFamily) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    switch (saFamily) {
        case AF_INET:
        case AF_UNIX:
            connectionManager->saFamily = saFamily;
            result = SAFU_RESULT_OK;
            break;
        default:
            safuLogErr("Unsupported address family");
            result = SAFU_RESULT_FAILED;
            break;
    }

    return result;
}

safuResultE_t elosConnectionManagerInitialize(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin,
                                              sa_family_t saFamily) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((connectionManager == NULL) || (plugin == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if (plugin->config == NULL) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == true) {
        safuLogErr("The given ConnectionManager is already initialized");
    } else {
        memset(connectionManager, 0, sizeof(elosConnectionManager_t));

        result = _initializeAddressFamily(connectionManager, saFamily);
        if (result == SAFU_RESULT_OK) {
            result = _initializeSharedData(connectionManager, plugin);
            if (result == SAFU_RESULT_OK) {
                result = _initializeListener(connectionManager, plugin);
                if (result == SAFU_RESULT_OK) {
                    result = _initializeConnections(connectionManager);
                    if (result == SAFU_RESULT_OK) {
                        connectionManager->syncFd = eventfd(0, 0);
                        if (connectionManager->syncFd == -1) {
                            safuLogErrErrnoValue("Creating eventfd failed", connectionManager->syncFd);
                        } else {
                            _initializeAuthorization(connectionManager, plugin->config);

                            atomic_store(&connectionManager->flags, SAFU_FLAG_INITIALIZED_BIT);
                        }
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosConnectionManagerDeleteMembers(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    if (connectionManager != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == true) {
            safuResultE_t iterResult;

            if (atomic_load(&connectionManager->flags) &
                (ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE | ELOS_CONNECTIONMANAGER_THREAD_NOT_JOINED)) {
                safuLogWarn(
                    "An attempt is being made to delete the ConnectionManager without it being properly terminated "
                    "first.");
            }

            for (int i = 0; i < ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT; i += 1) {
                elosClientConnection_t *connection = &connectionManager->connection[i];

                iterResult = elosClientConnectionDeleteMembers(connection);
                if (iterResult != SAFU_RESULT_OK) {
                    result = SAFU_RESULT_FAILED;
                }
            }

            retVal = sem_destroy(&connectionManager->sharedData.connectionSemaphore);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Destroying semaphore failed (possible memory leak)", retVal);
                result = SAFU_RESULT_FAILED;
            }

            elosClientAuthorizationDelete(&connectionManager->clientAuth);

            if (connectionManager->fd != -1) {
                retVal = close(connectionManager->fd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing listenFd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (connectionManager->syncFd != -1) {
                retVal = close(connectionManager->syncFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (connectionManager->saFamily == AF_UNIX) {
                struct sockaddr_un *addr = (struct sockaddr_un *)&connectionManager->addr;
                retVal = unlink(addr->sun_path);
                if (retVal != 0 && errno != ENOENT) {
                    safuLogErrErrnoValue("unlink socket path failed", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}
