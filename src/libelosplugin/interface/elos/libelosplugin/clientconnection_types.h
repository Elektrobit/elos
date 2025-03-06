// SPDX-License-Identifier: MIT
#pragma once

#include <elos/eventfilter/eventfilter_types.h>
#include <safu/flags.h>
#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <semaphore.h>

#include "types.h"

typedef safuVec_t elosEventFilterNodeIdVector_t;
typedef safuVec_t elosEventQueueIdVector_t;

/*******************************************************************
 * Data structures used exclusively by the worker thread
 *
 * Members:
 *   publisher: exclusiv for the connection
 *   subscriber: exclusiv for the connection
 ******************************************************************/
typedef struct elosClientConnectionData {
    struct elosPublisher *publisher;
    struct elosSubscriber *subscriber;
} elosClientConnectionData_t;

/*******************************************************************
 * Data structures shared between all connections of a ConnectionManager
 *
 * Members:
 *   connectionSemaphore: Used for waiting until a connection is available
 *   plugin: reference to the plugin instance
 ******************************************************************/
typedef struct elosClientConnectionSharedData {
    sem_t connectionSemaphore;
    elosPlugin_t *plugin;
} elosClientConnectionSharedData_t;

typedef struct elosClientConnection elosClientConnection_t;

typedef safuResultE_t (*elosClientConnectionInitializeConnection_t)(elosClientConnection_t *connection);
typedef safuResultE_t (*elosClientConnectionCloseConnection_t)(elosClientConnection_t *connection);
typedef safuResultE_t (*elosClientConnectionDeleteConnection_t)(elosClientConnection_t *connection);

/*******************************************************************
 * Data structure of a ClientConnection
 *
 * Members:
 *   flags: State bits of the component (e.g. initialized, active, e.t.c.)
 *   fd: The socket used for communication
 *   syncFd: eventfd used for synchronization with the worker thread
 *   triggerFd: eventfd used for waking the worker thread (e.g. for a controlled shutdown)
 *   clientConnectionContext: Pointer to data structure that holds plugin specific data
 *   thread: Worker thread used by pthread_* functions
 *   sharedData: See struct defintion of elosClientConnectionSharedData_t
 *   data: worker thread local data
 *   isTrusted: 'true' if the connection is trusted
 *   blacklist: blacklist filters
 *   initializeConnection: A pointer to a function that allocates plugin specific data structures
 *   closeConnection: A pointer to a function that closes the client connection
 *   deleteConnection: A pointer to a function that deletes allocated structures
 ******************************************************************/
typedef struct elosClientConnection {
    safuFlags_t flags;
    int fd;
    int syncFd;
    int triggerFd;
    void *clientConnectionContext;
    pthread_t thread;
    elosClientConnectionSharedData_t *sharedData;
    elosClientConnectionData_t data;
    bool isTrusted;
    elosEventFilter_t blacklist;
    elosClientConnectionInitializeConnection_t initializeConnection;
    elosClientConnectionCloseConnection_t closeConnection;
    elosClientConnectionDeleteConnection_t deleteConnection;
} elosClientConnection_t;
