// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>

#include "elos/eventprocessor/eventfilternode.h"

#define _EVENTQUEUE_PARAM {.elements = 1, .id = 42, .limitEvents = 0, .limitTime = {.tv_nsec = 0, .tv_sec = 0}};

typedef struct elosTestState {
    elosEventFilterNode_t eventFilterNode;
    elosEventFilterNodeParam_t param;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeProcessErrIter)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeProcessErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeProcessSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeProcessSuccessWithVStack)

void elosEventFilterNodeSetup(elosTestState_t *test, char const **filterString, uint32_t filterCount);
void elosEventFilterNodeTeardown(elosTestState_t *test);
void elosEventFilterNodeTestStep(elosTestState_t *test, char const *message, elosEvent_t const *event,
                                 elosEventFilterStack_t const *vStack, uint32_t queueElements, bool matchLastEvent);

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERNODE_PROCESS_UTEST_H */
