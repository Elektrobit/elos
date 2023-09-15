// SPDX-License-Identifier: MIT

#include "elosLog_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLogUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLogSuccess),
        TEST_CASE(elosTestElosLogLoggerUninitialized),
        TEST_CASE(elosTestElosLogInvalidLogger),
    };

    return RUN_TEST_SUITE(tests, elosLogUtest);
}

static int elosLogUtestSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosLogUtestTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
