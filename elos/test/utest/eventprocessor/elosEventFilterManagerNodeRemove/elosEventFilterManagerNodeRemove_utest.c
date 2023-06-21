// SPDX-License-Identifier: MIT
#include "elosEventFilterManagerNodeRemove_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveErrParam),
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveErrRemove),
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void elosTestCaseNew(void **state) {
    elosTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null(test);

    memset(test, 0, sizeof(elosTestState_t));

    elosEventQueueManagerParam_t eqmParam = {
        .defaultEventQueueSize = 1,
        .defaultLimitEvents = 1,
        .defaultLimitTime = {0},
    };
    result = elosEventQueueManagerInitialize(&test->eventQueueManager, &eqmParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventQueueManagerEntryNew(&test->eventQueueManager, &test->eventQueueId);
    assert_int_equal(result, SAFU_RESULT_OK);

    elosEventFilterManagerParam_t efmParam = {
        .elements = 2,
        .eventQueueManager = &test->eventQueueManager,
        .idManager = NULL,
    };
    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &efmParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    char const *filterStrings[] = {"1 1 EQ"};
    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueId, &test->eventFilterNodeIdA);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueId, &test->eventFilterNodeIdB);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterManagerNodeCreate(&test->eventFilterManager, filterStrings, ARRAY_SIZE(filterStrings),
                                              test->eventQueueId, &test->eventFilterNodeIdC);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;
}

void elosTestCaseDelete(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    elosEventQueueManagerDeleteMembers(&test->eventQueueManager);
    free(*state);
    *state = NULL;
}