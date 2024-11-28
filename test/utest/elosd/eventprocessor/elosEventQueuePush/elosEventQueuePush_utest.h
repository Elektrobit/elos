// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <errno.h>

#include "elos/eventprocessor/eventqueue.h"

#define _EVENTQUEUE_PARAM {.elements = 1, .id = 42, .limitEvents = 0, .limitTime = {.tv_nsec = 0, .tv_sec = 0}};

typedef struct elosTestState {
    elosEventQueue_t eventQueue;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueuePushErrDeepCopy)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueuePushErrMutex)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueuePushErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueuePushErrVecPush)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueuePushSuccess)

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H */
