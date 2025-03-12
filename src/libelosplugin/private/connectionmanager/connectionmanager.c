// SPDX-License-Identifier: MIT

#include <arpa/inet.h>
#include <elos/libelosplugin/clientauthorization_types.h>
#include <elos/libelosplugin/clientauthorizedprocesses.h>
#include <elos/libelosplugin/clientblacklist.h>
#include <elos/libelosplugin/clientconnection.h>
#include <elos/libelosplugin/connectionmanager.h>
#include <elos/libelosplugin/types.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <stdlib.h>
#include <sys/eventfd.h>

#include "connectionmanager_private.h"

static safuResultE_t _initializeSharedData(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientConnectionSharedData_t *sharedData = &connectionManager->sharedData;
    int retVal;

    retVal = sem_init(&sharedData->connectionSemaphore, 0, connectionManager->connectionLimit);
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

    if (connectionManager->initializeListener != NULL) {
        result = connectionManager->initializeListener(connectionManager, plugin);
    }

    if (result != SAFU_RESULT_OK) {
        safuLogErr("Initialization of Listener failed");
    }

    return result;
}

static safuResultE_t _initializeConnections(elosConnectionManager_t *connectionManager) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    for (int i = 0; i < connectionManager->connectionLimit; i += 1) {
        elosClientConnection_t *connection = &connectionManager->connections[i];

        result = elosClientConnectionInitialize(connection, &connectionManager->sharedData);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Initialization of connection data structure [%d] failed", i);
            break;
        }
        if (connectionManager->setConnectionHandlers != NULL) {
            result = connectionManager->setConnectionHandlers(connection);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Setting handlers for connection [%d] failed", i);
                break;
            }
        }
        if (connection->initializeConnection != NULL) {
            result = connection->initializeConnection(connection);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Initialization of connection [%d] failed", i);
                break;
            }
        }
    }

    return result;
}

static safuResultE_t _initializeAuthorization(elosConnectionManager_t *connectionManager,
                                              samconfConfig_t const *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connectionManager->authorizationInitialize != NULL) {
        result = connectionManager->authorizationInitialize(&connectionManager->clientAuth);
    }

    if (result != SAFU_RESULT_OK) {
        safuLogWarn("Client Authorization Initialize failed");
    } else {
        result = elosAuthorizedProcessInitialize(&connectionManager->clientAuth.authorizedProcessFilters, config);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("authorized process filter initialization failed");
        } else {
            safuLogDebug("authorized process filter initialization succeeded");
        }
    }

    return result;
}

static safuResultE_t _initializeConnectionLimit(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connectionManager->getConnectionLimit != NULL) {
        connectionManager->connectionLimit = connectionManager->getConnectionLimit(plugin);
        result = SAFU_RESULT_OK;
    } else {
        safuLogInfoF("No connection limit is specified, setting limit to default (%d)",
                     ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT);
        connectionManager->connectionLimit = ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT;
    }

    if ((unsigned)connectionManager->connectionLimit > ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT) {
        safuLogErrF("Number of connections exceed limits, applying limit of %i",
                    ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT);
        connectionManager->connectionLimit = ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT;
    }

    return result;
}

safuResultE_t elosConnectionManagerInitialize(elosConnectionManager_t *connectionManager, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((connectionManager == NULL) || (plugin == NULL)) {
        safuLogErr("Invalid parameter NULL");
    } else if (plugin->config == NULL) {
        safuLogErr("Invalid value NULL in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&connectionManager->flags) == true) {
        safuLogErr("The given ConnectionManager is already initialized");
    } else {
        result = _initializeConnectionLimit(connectionManager, plugin);
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
                elosClientConnection_t *connection = &connectionManager->connections[i];

                iterResult = elosClientConnectionDeleteMembers(connection);
                if (iterResult != SAFU_RESULT_OK) {
                    result = SAFU_RESULT_FAILED;
                }
                if (connection->deleteConnection != NULL) {
                    result = connection->deleteConnection(connection);
                    if (iterResult != SAFU_RESULT_OK) {
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }

            retVal = sem_destroy(&connectionManager->sharedData.connectionSemaphore);
            if (retVal != 0) {
                safuLogWarnErrnoValue("Destroying semaphore failed (possible memory leak)", retVal);
                result = SAFU_RESULT_FAILED;
            }

            if (connectionManager->syncFd != -1) {
                retVal = close(connectionManager->syncFd);
                if (retVal != 0) {
                    safuLogWarnErrnoValue("Closing eventfd failed (possible memory leak)", retVal);
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (connectionManager->authorizationDelete != NULL) {
                result = connectionManager->authorizationDelete(&connectionManager->clientAuth);
            } else {
                result = SAFU_RESULT_FAILED;
            }
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Deletion of client authorization failed");
            }

            if (connectionManager->deleteListener != NULL) {
                result = connectionManager->deleteListener(connectionManager);
            } else {
                result = SAFU_RESULT_FAILED;
            }
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Deletion of connection failed");
            }
        }
    }

    return result;
}
