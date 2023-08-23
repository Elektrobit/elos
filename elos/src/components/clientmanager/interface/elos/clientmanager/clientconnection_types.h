// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>
#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <semaphore.h>

#include "elos/clientmanager/clientconnection_defines.h"
#include "elos/eventdispatcher/types.h"
#include "elos/eventfilter/eventfilter_types.h"
#include "elos/eventlogging/LogAggregatorTypes.h"
#include "elos/eventprocessor/types.h"

typedef safuVec_t elosEventFilterNodeIdVector_t;
typedef safuVec_t elosEventQueueIdVector_t;

/*******************************************************************
 * Data structures used exclusively by the worker thread
 *
 * Members:
 *   eventFilterNodeIdVector: FilterNodes used by the connection
 *   eventQueueIdVector: EventQueues used by the connection
 ******************************************************************/
typedef struct elosClientConnectionData {
    elosEventFilterNodeIdVector_t eventFilterNodeIdVector;
    elosEventQueueIdVector_t eventQueueIdVector;
} elosClientConnectionData_t;

/*******************************************************************
 * Data structures shared between all connections of a ClientManager
 *
 * Members:
 *   logAggregator: Used for persistent logging of Events
 *   connectionSemaphore: Used for waiting until a connection is available
 *   eventDispatcher: Used for registering the EventBuffers of each Connection
 *   eventProcessor: Used for FilterNode/EventQueue handling
 *   config: Static configuration variables
 ******************************************************************/
typedef struct elosClientConnectionSharedData {
    elosLogAggregator_t *logAggregator;
    sem_t connectionSemaphore;
    elosEventDispatcher_t *eventDispatcher;
    elosEventProcessor_t *eventProcessor;
    samconfConfig_t *config;
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
 *   eventBuffer: Buffer for incoming Events.
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
    elosEventBuffer_t eventBuffer;
} elosClientConnection_t;

/*******************************************************************
 * Initialization parameters for a new ClientConnection
 *
 * Members:
 *   sharedData: See definition of elosClientConnectionSharedData_t
 ******************************************************************/
typedef struct elosClientConnectionParam {
    elosClientConnectionSharedData_t *sharedData;
} elosClientConnectionParam_t;
