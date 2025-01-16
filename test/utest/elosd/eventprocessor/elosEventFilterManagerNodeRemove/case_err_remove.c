// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterManagerNodeRemoveErrRemoveSetup(void **state) {
    elosTestCaseNew(state);
    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveErrRemoveTeardown(void **state) {
    elosTestCaseDelete(state);
    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveErrRemove(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "fail as elosEventFilterNodeVectorRemoveById fails");

    MOCK_FUNC_ENABLE(elosEventFilterNodeVectorRemoveById);
    expect_value(elosEventFilterNodeVectorRemoveById, eventFilterNodeVector,
                 &test->eventFilterManager.eventFilterNodeVector);
    expect_value(elosEventFilterNodeVectorRemoveById, id, test->eventFilterNodeIdA);
    will_return(elosEventFilterNodeVectorRemoveById, SAFU_RESULT_FAILED);

    result = elosEventFilterManagerNodeRemove(&test->eventFilterManager, test->eventFilterNodeIdA);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
