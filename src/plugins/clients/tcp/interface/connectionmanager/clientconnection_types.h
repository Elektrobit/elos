// SPDX-License-Identifier: MIT
#pragma once

#include <elos/eventfilter/eventfilter_types.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <netinet/in.h>
#include <safu/flags.h>
#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <semaphore.h>

#include "connectionmanager/clientconnection_defines.h"

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

/*******************************************************************
 * Data structure of a ClientConnection
 *
 * Members:
 *   flags: State bits of the component (e.g. initialized, active, e.t.c.)
 *   fd: The socket used for communication
 *   syncFd: eventfd used for synchronization with the worker thread
 *   triggerFd: eventfd used for waking the worker thread (e.g. for a controlled shutdown)
 *   addr: Address information of the currently used socketFd
 *   thread: Worker thread used by pthread_* functions
 *   sharedData: See struct defintion of elosClientConnectionSharedData_t
 *   data: worker thread local data
 *   isTrusted: 'true' if the connection is trusted
 *   blacklist: blacklist filters
 ******************************************************************/
typedef struct elosClientConnection {
    safuFlags_t flags;
    int fd;
    int syncFd;
    int triggerFd;
    struct sockaddr_in addr;
    pthread_t thread;
    elosClientConnectionSharedData_t *sharedData;
    elosClientConnectionData_t data;
    bool isTrusted;
    elosEventFilter_t blacklist;
} elosClientConnection_t;
