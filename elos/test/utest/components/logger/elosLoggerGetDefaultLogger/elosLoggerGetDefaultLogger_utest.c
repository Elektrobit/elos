// SPDX-License-Identifier: MIT

#include "elosLoggerGetDefaultLogger_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLoggerGetDefaultLoggerUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoggerGetDefaultLoggerSuccess),
        TEST_CASE(elosTestElosLoggerGetDefaultLoggerErrFunc),
    };

    return RUN_TEST_SUITE(tests, elosLoggerGetDefaultLoggerUtest);
}

static int elosLoggerGetDefaultLoggerUtestSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosLoggerGetDefaultLoggerUtestTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
