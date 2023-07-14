// SPDX-License-Identifier: MIT
#include "elosEventBufferWrite_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventBufferWriteErrFunc),
        TEST_CASE(elosTestElosEventBufferWriteErrParam),
        TEST_CASE(elosTestElosEventBufferWriteSuccessThreaded),
        TEST_CASE(elosTestElosEventBufferWriteSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
