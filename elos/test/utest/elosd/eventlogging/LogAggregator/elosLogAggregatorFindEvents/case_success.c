// SPDX-License-Identifier: MIT

#include "elosLogAggregatorFindEvents_utest.h"

int elosTestElosLogAggregatorFindEventsSuccessSetup(void **state) {
    elosUteststateT_t *testState;

    testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);
    memset(testState, 0, sizeof(elosUteststateT_t));

    *state = testState;

    elosLogAggregatorFindEventsUtestCreateEvent(state);
    elosLogAggregatorFindEventsUtestCreateFilterRules(state);
    elosLogAggregatorFindEventsUtestCreateLogAggregator(state);
    elosLogAggregatorFindEventsUtestCreateVector(state);

    return 0;
}

int elosTestElosLogAggregatorFindEventsSuccessTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    elosLogAggregatorFindEventsUtestFreeEvent(state);
    elosLogAggregatorFindEventsUtestFreeFilterRules(state);
    elosLogAggregatorFindEventsUtestFreeLogAggregator(state);
    elosLogAggregatorFindEventsUtestFreeVector(state);
    free(testState);

    return 0;
}

void elosTestElosLogAggregatorFindEventsSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    const char **filterRules = testState->filterRules;
    safuResultE_t result;

    TEST("elosLogAggregatorFindEvents");
    SHOULD("%s", "test if filtering events from logAggregator works properly");

    print_message("Test event: %s\n", testState->eventJsonString);

    for (int idx = 0; idx < NUM_FILTER; idx++) {
        PARAM("filterrule: %s", filterRules[idx]);

        result = elosLogAggregatorFindEvents(&testState->logAggregator, filterRules[idx], &testState->eventVector);
        assert_int_not_equal(result, SAFU_RESULT_FAILED);
    }

    // 2 out of 3 filter shall match the event
    assert_int_equal(testState->eventVector.elementCount, 2);
}
