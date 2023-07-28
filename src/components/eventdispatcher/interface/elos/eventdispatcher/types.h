// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <time.h>

#include "elos/eventbuffer/types.h"
#include "elos/eventprocessor/types.h"

/*******************************************************************
 * Parameter for EventDispatcher initialization
 *
 * Members:
 *   eventProcessor: The EventProcessor the Events shall be dispatched to
 *   pollTimeout:  Timeout for the poll() command that is waiting for eventfd writes
 *                 Can be NULL, in this case ELOS_EVENTDISPATCHER_DEFAULT_POLL_TIMEOUT is used.
 *   healthTimeInterval:  Minimum time interval for health events to be published (can be more).
 *                        Can be NULL, in this case ELOS_EVENTDISPATCHER_DEFAULT_HEALTH_INTERVAL is used.
 ******************************************************************/
typedef struct elosEventDispatcherParam {
    elosEventProcessor_t *eventProcessor;
    struct timespec const *pollTimeout;
    struct timespec const *healthTimeInterval;
} elosEventDispatcherParam_t;

/*******************************************************************
 * Data structure of an EventDispatcher's worker thread
 *
 * Members:
 *   thread: Posix thread identifiert
 *   trigger: Eventfd used for synchronization between worker thread and the EventDispatcher
 *   pollTimeout: Timeout for the poll() command that is waiting for eventfd writes
 *   healthTimeInterval: Minimum time interval for health events to be published (can be more)
 *   healthTimeTreshold: Timestamp used for testing if a health event needs to be published
 *   eventsPublished: Published events since the last health event
 ******************************************************************/
typedef struct elosEventDispatcherWorker {
    pthread_t thread;
    int trigger;
    struct timespec pollTimeout;
    struct timespec healthTimeInterval;
    struct timespec healthTimeTreshold;
    size_t eventsPublished;
} elosEventDispatcherWorker_t;

/*******************************************************************
 * Data structure of an EventDispatcher
 *
 * Members:
 *   flags: contains the component status bits, e.g. initialized, active
 *   lock: mutex for ensuring thread safe access to the EventDispatcher
 *   sync: eventfd used for synchronization between EventDispatcher and its worker thread
 *   worker: data structure for the EventDispatcher worker thread
 *   eventBufferPtrVector: contains the EventBuffers the EventDispatcher is monitoring
 *   eventProcessor: the EventProcessor the Events shall be dispatched to
 ******************************************************************/
typedef struct elosEventDispatcher {
    safuFlags_t flags;
    pthread_mutex_t lock;
    int sync;
    elosEventDispatcherWorker_t worker;
    elosEventBufferPtrVector_t eventBufferPtrVector;
    elosEventProcessor_t *eventProcessor;
} elosEventDispatcher_t;
