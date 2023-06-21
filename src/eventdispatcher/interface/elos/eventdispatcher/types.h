// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>

#include "elos/eventbuffer/types.h"
#include "elos/eventprocessor/types.h"

/*******************************************************************
 * Parameter for EventDispatcher initialization
 *
 * Members:
 *   eventProcessor: the EventProcessor the Events shall be dispatched to
 ******************************************************************/
typedef struct elosEventDispatcherParam {
    elosEventProcessor_t *eventProcessor;
} elosEventDispatcherParam_t;

/*******************************************************************
 * Data structure of an EventDispatcher's worker thread
 *
 * Members:
 *   thread: posix thread identifiert
 *   sync: eventfd used for synchronization between worker thread and the EventDispatcher
 ******************************************************************/
typedef struct elosEventDispatcherWorker {
    pthread_t thread;
    int sync;
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
