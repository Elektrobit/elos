// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"

#define _EVENTBUFFER_ENTRIES 100
#define _EVENT_ENTRIES       ELOS_EVENTBUFFER_PRIORITY_COUNT

#define _EVENT_IDX_NORMAL     0
#define _EVENT_IDX_HIGH       1
#define _EVENT_PAYLOAD_HIGH   "eventPriorityHigh"
#define _EVENT_PAYLOAD_NORMAL "eventPriorityNormal"

#define _TEST_ELEMENTS          (_EVENT_ENTRIES)
#define _TEST_WORKERS           (_EVENT_ENTRIES * _EVENTBUFFER_ENTRIES)
#define _TEST_WORKER_ITERATIONS (100)
#define _TEST_TOTAL_ITERATIONS  (_TEST_WORKERS * _TEST_WORKER_ITERATIONS)

typedef struct elosUnitTestWorker {
    pthread_t thread;
    int priority;
    elosEventBuffer_t *eventBuffer;
} elosUnitTestWorker_t;

typedef struct elosUnitTestState {
    elosEventDispatcher_t eventDispatcher;
    elosEventProcessor_t eventProcessor;
    elosEventBuffer_t eventBuffer[_EVENTBUFFER_ENTRIES];
    elosUnitTestWorker_t worker[_TEST_WORKERS];
} elosUnitTestState_t;

typedef struct elosEventCount {
    atomic_uint high;
    atomic_uint normal;
    atomic_uint health;
} elosEventCount_t;

// Temporary bandaid that should be replaced with a proper mocking solution (function overloading) in the future
typedef struct elosEventProcessorPublishBandaid {
    safuResultE_t result;
    elosEventCount_t eventCount;
} elosEventProcessorPublishBandaid_t;

extern elosEventProcessorPublishBandaid_t elosEventProcessorPublishBandaid;

TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherWorkerSuccess)
