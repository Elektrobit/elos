// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/event/event.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "mock_eventbuffer.h"

#define _EVENTBUFFER_ENTRIES 5
#define _EVENT_ENTRIES       2

#define _EVENT_IDX_NORMAL     0
#define _EVENT_IDX_HIGH       1
#define _EVENT_PAYLOAD_HIGH   "eventPriorityHigh"
#define _EVENT_PAYLOAD_NORMAL "eventPriorityNormal"

typedef struct elosUnitTestState {
    elosEventDispatcher_t eventDispatcher;
    elosEventProcessor_t eventProcessor;
    elosEventBuffer_t eventBuffer[_EVENTBUFFER_ENTRIES];
    elosEvent_t event[_EVENT_ENTRIES];
} elosUnitTestState_t;

typedef struct elosEventCount {
    size_t high;
    size_t normal;
} elosEventCount_t;

// Temporary bandaid that should be replaced with a proper mocking solution (function overloading) in the future
typedef struct elosEventProcessorPublishBandaid {
    safuResultE_t result;
    elosEventCount_t eventCount;
} elosEventProcessorPublishBandaid_t;

extern elosEventProcessorPublishBandaid_t elosEventProcessorPublishBandaid;

void elosTestEventBufferReset(elosUnitTestState_t *test);

TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherDispatchErrFunc)
TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherDispatchErrMutex)
TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherDispatchErrParam)
TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherDispatchSuccess)
