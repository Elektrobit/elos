// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYPROCESS_UTEST_H
#define ELOS_EVENTFILTERENTRYPROCESS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "eventprocessor/eventfilternodevector.h"

#define _EQ_PARAM(__id) {.elements = 1, .id = (__id), .limitEvents = 0, .limitTime = {.tv_nsec = 0, .tv_sec = 0}};

typedef struct elosTestState {
    elosEventFilterNodeVector_t eventFilterNodeVector;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorProcessErrIter)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorProcessErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorProcessSuccessWithVStack)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorProcessSuccess)

void elosEventFilterNodeVectorSetup(elosTestState_t *test, char const *fStrA, char const *fStrB);
void elosEventFilterNodeVectorTeardown(elosTestState_t *test);
void elosEventFilterNodeVectorTestStep(elosTestState_t *test, char const *message, elosEvent_t const *event,
                                       elosEventFilterStack_t const *vStack, uint32_t sizeA, uint32_t sizeB);

#endif /* ELOS_EVENTFILTERENTRYPROCESS_UTEST_H */
