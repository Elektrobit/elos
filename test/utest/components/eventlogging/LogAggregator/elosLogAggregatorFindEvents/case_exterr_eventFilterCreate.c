// SPDX-License-Identifier: MIT

#include "elosLogAggregatorFindEvents_utest.h"
#include "mock_eventfilter.h"

int elosTestElosLogAggregatorFindEventsExterrEventFilterCreateSetup(void **state) {
    elosUteststateT_t *testState;

    testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);
    memset(testState, 0, sizeof(elosUteststateT_t));

    *state = testState;
    return 0;
}

int elosTestElosLogAggregatorFindEventsExterrEventFilterCreateTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState);
    return 0;
}

void elosTestElosLogAggregatorFindEventsExterrEventFilterCreate(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosEventFilter_t filter = ELOS_EVENTFILTER_INIT;

    const char *filterRule = ".event.source.appName 'testApp' STRCMP";

    TEST("elosLogAggregatorFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventFilterCreate failed");

    MOCK_FUNC_AFTER_CALL(elosEventFilterCreate, 0);
    expect_any(elosEventFilterCreate, filter);
    expect_any(elosEventFilterCreate, param);
    will_set_parameter(elosEventFilterCreate, filter, &filter);
    will_return(elosEventFilterCreate, RPNFILTER_RESULT_ERROR);

    result = elosLogAggregatorFindEvents(&testState->logAggregator, filterRule, &testState->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
