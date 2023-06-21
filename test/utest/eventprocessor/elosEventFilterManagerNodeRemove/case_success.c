// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterManagerNodeRemoveSuccessSetup(void **state) {
    elosTestCaseNew(state);
    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveSuccessTeardown(void **state) {
    elosTestCaseDelete(state);
    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "test correct behaviour of elosEventFilterManagerNodeRemove");

    PARAM("%s", "delete first eventFilterNode");
    result = elosEventFilterManagerNodeRemove(&test->eventFilterManager, test->eventFilterNodeIdB);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventFilterManager.eventFilterNodeVector.elementCount, 2);

    PARAM("%s", "delete second eventFilterNode");
    result = elosEventFilterManagerNodeRemove(&test->eventFilterManager, test->eventFilterNodeIdA);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventFilterManager.eventFilterNodeVector.elementCount, 1);

    PARAM("%s", "delete third eventFilterNode");
    result = elosEventFilterManagerNodeRemove(&test->eventFilterManager, test->eventFilterNodeIdC);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventFilterManager.eventFilterNodeVector.elementCount, 0);
}