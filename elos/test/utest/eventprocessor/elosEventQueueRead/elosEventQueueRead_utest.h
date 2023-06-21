// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTQUEUE_READ_UTEST_H
#define ELOS_EVENTPROCESSOR_EVENTQUEUE_READ_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <errno.h>

#include "elos/eventprocessor/eventqueue.h"

#define _EVENTQUEUE_PARAM {.elements = 1, .id = 42, .limitEvents = 0, .limitTime = {.tv_nsec = 0, .tv_sec = 0}};

typedef struct elosUnitTestState {
    elosEventQueue_t eventQueue;
    elosEventVector_t *eventVector;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueReadErrFunc)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueReadErrMutex)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueReadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueReadSuccess)

#endif /* ELOS_EVENTPROCESSOR_EVENTQUEUE_READ_UTEST_H */
