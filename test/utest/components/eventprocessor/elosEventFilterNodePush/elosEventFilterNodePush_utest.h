// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERNODEPUSH_UTEST_H
#define ELOS_EVENTFILTERNODEPUSH_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfilternode.h"

typedef struct elosTestState {
    elosEventFilterNode_t efe;
    elosEventFilter_t filter1;
    elosEventFilter_t filter2;
    elosEventFilterParam_t param1;
    elosEventFilterParam_t param2;
    char *filterString1;
    char *filterString2;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushCountZero)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushEfeNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushFilterNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushPushFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushCreateFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodePushFilterStringNull)

#endif /* ELOS_EVENTFILTERNODEPUSH_UTEST_H */
