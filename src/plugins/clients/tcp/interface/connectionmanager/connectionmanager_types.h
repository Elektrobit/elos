// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include <safu/flags.h>
#include <samconf/samconf_types.h>

#include "connectionmanager/clientauthorization_types.h"
#include "connectionmanager/clientconnection_types.h"

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
 ******************************************************************/
typedef struct elosConnectionManager {
    safuFlags_t flags;
    int fd;
    int syncFd;
    struct sockaddr_in addr;
    elosClientConnection_t connection[ELOS_CONNECTIONMANAGER_CONNECTION_LIMIT];
    pthread_t listenThread;
    elosClientConnectionSharedData_t sharedData;
    elosClientAuthorization_t clientAuth;
} elosConnectionManager_t;
