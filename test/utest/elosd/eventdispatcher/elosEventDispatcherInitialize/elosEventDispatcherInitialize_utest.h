// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventdispatcher/eventdispatcher.h"

typedef struct elosUnitTestState {
    elosEventDispatcher_t eventDispatcher;
    elosEventProcessor_t eventProcessor;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherInitializeErrMutex)
TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(testElosEventDispatcherInitializeSuccess)
