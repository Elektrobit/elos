// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include <safu/flags.h>
#include <samconf/samconf_types.h>
#include <sys/un.h>

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

typedef safuResultE_t (*elosInitializeListener_t)(elosConnectionManager_t *connectionManager,
                                                  elosPlugin_t const *plugin);
typedef int (*elosGetConnectionLimit_t)(elosPlugin_t const *plugin);
typedef safuResultE_t (*elosClientAuthorizationInitialize_t)(elosClientAuthorization_t *const clientAuth);
typedef safuResultE_t (*elosClientAuthorizationDelete_t)(elosClientAuthorization_t *const clientAuth);
typedef bool (*elosClientAuthorizationIsValid_t)(elosClientAuthorization_t *const clientAuth,
                                                 struct sockaddr const *const addr);

/*******************************************************************
 * Data structure of a ConnectionManager

 * Members:
 *   flags: State bits of the component (e.g. initialized, active, e.t.c.)
 *   fd: listener socket used for waiting for new connections
 *   syncFd: eventfd used for synchronization with the worker thread
 *   addr: Address information of the listener socket
 *   connection: Array of ClientConnections
 *   listenThread: worker thread used by pthread_* functions
 *   sharedData: Data shared between all ClientConnections
 *   clientAuth: Client authorization functionality
 *   initializeListener: Pointer to a function that is setting up the listener socket
 *   getConnectionLimit: Pointer to a function that is getting the maximum of client connection from config
 *   authorizationInitialize: Pointer to a function that is initializing data structures for the client authorization
 *   authorizationDelete: Pointer to a function that is deleting data structures for the client authorization
 *   authorizationIsValid: Pointer to a function that is checking if a client has authorization
 ******************************************************************/
typedef struct elosConnectionManager {
    safuFlags_t flags;
    int fd;
    int syncFd;
    union {
        struct sockaddr_in tcpAddr;
        struct sockaddr_un unixAddr;
    } addr;
    int connectionLimit;
    elosClientConnection_t connection[ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT];
    pthread_t listenThread;
    elosClientConnectionSharedData_t sharedData;
    elosClientAuthorization_t clientAuth;
    elosInitializeListener_t initializeListener;
    elosGetConnectionLimit_t getConnectionLimit;
    elosClientAuthorizationInitialize_t authorizationInitialize;
    elosClientAuthorizationDelete_t authorizationDelete;
    elosClientAuthorizationIsValid_t authorizationIsValid;
} elosConnectionManager_t;
