// SPDX-License-Identifier: MIT

#include "elosLogAggregatorFindEvents_utest.h"

int elosTestElosLogAggregatorFindEventsErrParamLogAggregatorNullSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);
    memset(testState, 0, sizeof(elosUteststateT_t));

    *state = testState;
    return 0;
}

int elosTestElosLogAggregatorFindEventsErrParamLogAggregatorNullTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState);
    return 0;
}

void elosTestElosLogAggregatorFindEventsErrParamLogAggregatorNull(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    const char *filterRule = ".event.source.appName 'testApp' STRCMP";

    TEST("elosLogAggregatorFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when parameter logAggregator is null");

    result = elosLogAggregatorFindEvents(NULL, filterRule, &testState->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
