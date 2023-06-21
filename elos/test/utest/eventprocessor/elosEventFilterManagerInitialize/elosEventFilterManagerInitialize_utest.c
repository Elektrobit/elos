// SPDX-License-Identifier: MIT
#include "elosEventFilterManagerInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterManagerInitializeErrParam),
        TEST_CASE(elosTestEloEventFilterManagerInitializeSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    elosEventQueueManagerParam_t eqmParam = {
        .defaultEventQueueSize = 1,
        .defaultLimitEvents = 1,
        .defaultLimitTime = {0},
    };
    result = elosEventQueueManagerInitialize(&test->eqm, &eqmParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(*state);
    *state = NULL;

    return 0;
}
