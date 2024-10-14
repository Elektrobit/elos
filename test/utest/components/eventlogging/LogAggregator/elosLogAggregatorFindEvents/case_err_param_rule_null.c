// SPDX-License-Identifier: MIT

#include "elosLogAggregatorFindEvents_utest.h"

int elosTestElosLogAggregatorFindEventsErrParamRuleNullSetup(void **state) {
    elosUteststateT_t *testState;

    testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);
    memset(testState, 0, sizeof(elosUteststateT_t));

    *state = testState;
    return 0;
}

int elosTestElosLogAggregatorFindEventsErrParamRuleNullTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState);
    return 0;
}

void elosTestElosLogAggregatorFindEventsErrParamRuleNull(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosLogAggregatorFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when parameter rule is null");

    struct timespec nullTime = {0};
    result =
        elosLogAggregatorFindEvents(&testState->logAggregator, NULL, &nullTime, &nullTime, &testState->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
