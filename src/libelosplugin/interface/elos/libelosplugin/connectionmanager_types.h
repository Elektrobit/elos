// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <samconf/samconf_types.h>

#include "clientauthorization_types.h"
#include "clientconnection_types.h"

#define ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT    200
#define ELOS_CONNECTIONMANAGER_LISTEN_QUEUE_LENGTH 200

#define ELOS_CONNECTIONMANAGER_LISTEN_ACTIVE     (SAFU_FLAG_CUSTOM_START_BIT << 0)
#define ELOS_CONNECTIONMANAGER_CONNECTION_ACTIVE (SAFU_FLAG_CUSTOM_START_BIT << 1)
#define ELOS_CONNECTIONMANAGER_THREAD_NOT_JOINED (SAFU_FLAG_CUSTOM_START_BIT << 2)

#ifndef ELOS_CONNECTIONMANAGER_EVENTFILTERNODEIDVECTOR_SIZE
#define ELOS_CONNECTIONMANAGER_EVENTFILTERNODEIDVECTOR_SIZE 4
#endif

#ifndef ELOS_CONNECTIONMANAGER_EVENTQUEUEIDVECTOR_SIZE
#define ELOS_CONNECTIONMANAGER_EVENTQUEUEIDVECTOR_SIZE 4
#endif

typedef struct elosConnectionManager elosConnectionManager_t;

typedef safuResultE_t (*elosConnectionManagerInitializeListener_t)(elosConnectionManager_t *connectionManager,
                                                                   elosPlugin_t const *plugin);
typedef safuResultE_t (*elosConnectionManagerListenerAcceptConnection_t)(elosConnectionManager_t *connectionManager,
                                                                         elosClientConnection_t *connection);
typedef safuResultE_t (*elosConnectionManagerCloseListener_t)(elosConnectionManager_t *connectionManager);
typedef safuResultE_t (*elosConnectionManagerDeleteListener_t)(elosConnectionManager_t *connectionManager);

typedef safuResultE_t (*elosClientAuthorizationInitialize_t)(elosClientAuthorization_t *const clientAuth);
typedef safuResultE_t (*elosClientAuthorizationDelete_t)(elosClientAuthorization_t *const clientAuth);
typedef safuResultE_t (*elosClientAuthorizationIsValid_t)(elosConnectionManager_t *connectionManager,
                                                          elosClientConnection_t *connection);

typedef safuResultE_t (*elosConnectionManagerSetClientConnectionHandlers_t)(elosClientConnection_t *connection);
typedef int (*elosGetConnectionLimit_t)(elosPlugin_t const *plugin);

/*******************************************************************
 * Data structure of a ConnectionManager

 * Members:
 *   flags: State bits of the component (e.g. initialized, active, e.t.c.)
 *   fd: listener socket used for waiting for new connections
 *   syncFd: eventfd used for synchronization with the worker thread *
 *   connections: Array of ClientConnections
 *   listenThread: worker thread used by pthread_* functions
 *   sharedData: Data shared between all ClientConnections
 *   clientAuth: Client authorization functionality
 *   setConnectionHandlers: Pointer to a function that sets connection specific function handlers
 *   connectionManagerContext: Pointer to data structure that holds plugin specific data
 *   initializeListener: Pointer to a function that sets up the listener socket
 *   accept: A pointer to a function that accepts incoming connections on the listener socket
 *   closeListener: A pointer to a function that closes the listener socket
 *   deleteListener: A pointer to a function that deletes allocated structures
 *   authorizationInitialize: Pointer to a function that initializes data structures for the client authorization
 *   authorizationDelete: Pointer to a function that deletes data structures for the client authorization
 *   authorize: Pointer to a function that checks whether a client is authorized
 *   getConnectionLimit: Pointer to a function that gets the maximum number of client connections from config
 ******************************************************************/
typedef struct elosConnectionManager {
    safuFlags_t flags;
    int fd;
    int syncFd;
    int connectionLimit;
    elosClientConnection_t connections[ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT];
    pthread_t listenThread;
    elosClientConnectionSharedData_t sharedData;
    elosClientAuthorization_t clientAuth;
    void *connectionManagerContext;
    elosConnectionManagerSetClientConnectionHandlers_t setConnectionHandlers;
    elosConnectionManagerInitializeListener_t initializeListener;
    elosConnectionManagerListenerAcceptConnection_t accept;
    elosConnectionManagerCloseListener_t closeListener;
    elosConnectionManagerDeleteListener_t deleteListener;
    elosClientAuthorizationInitialize_t authorizationInitialize;
    elosClientAuthorizationDelete_t authorizationDelete;
    elosClientAuthorizationIsValid_t authorize;
    elosGetConnectionLimit_t getConnectionLimit;
} elosConnectionManager_t;
