// SPDX-License-Identifier: MIT
#pragma once

#include <safu/flags.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>
#include <stdatomic.h>

#include "elos/common/types.h"
#include "elos/event/event_types.h"
#include "elos/eventprocessor/eventfilter_types.h"
#include "elos/eventprocessor/eventqueue_types.h"

typedef struct elosIdManagerParam {
    void *placeholder;  // Will be replaced with proper values later
} elosIdManagerParam_t;

typedef struct elosIdManager {
    void *placeholder;  // Will be replaced with proper values later
} elosIdManager_t;

typedef struct elosEventQueueManagerParam {
    size_t defaultEventQueueSize;                    // x == 0 -> ELOS_EVENTQUEUE_DEFAULT_SIZE
    elosEventQueueLimitEvents_t defaultLimitEvents;  // x == 0 -> limit disabled
    elosEventQueueLimitTime_t defaultLimitTime;      // x == 0 -> limit disabled
} elosEventQueueManagerParam_t;

typedef struct elosEventQueueManager {
    safuFlags_t flags;
    pthread_mutex_t mutex;
    elosIdManager_t *idManager;
    uint32_t idCount;                         // Will be replaced with IdManager implementation later
    elosEventQueueVector_t eventQueueVector;  // contains elements of elosEventQueue_t
    size_t defaultEventQueueSize;
    elosEventQueueLimitEvents_t defaultLimitEvents;
    elosEventQueueLimitTime_t defaultLimitTime;
} elosEventQueueManager_t;

typedef struct elosEventFilterManagerParam {
    elosIdManager_t *idManager;
    elosEventQueueManager_t *eventQueueManager;
    size_t elements;
} elosEventFilterManagerParam_t;

typedef struct elosEventFilterManager {
    safuFlags_t flags;
    pthread_mutex_t mutex;
    elosIdManager_t *idManager;
    elosEventQueueManager_t *eventQueueManager;
    elosEventFilterNodeVector_t eventFilterNodeVector;
    elosEventFilterNodeId_t idCount;  // Temporary solution, will be removed once idManager is implemented
} elosEventFilterManager_t;

typedef struct elosEventProcessorParam {
    samconfConfig_t *config;
} elosEventProcessorParam_t;

typedef struct elosEventProcessor {
    elosEventFilterManager_t eventFilterManager;
    elosEventQueueManager_t eventQueueManager;
} elosEventProcessor_t;
