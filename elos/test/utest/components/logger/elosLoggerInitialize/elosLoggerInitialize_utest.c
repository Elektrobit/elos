// SPDX-License-Identifier: MIT

#include "elosLoggerInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLoggerInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoggerInitializeSuccess),
        TEST_CASE(elosTestElosLoggerInitializeErrFunc),
    };

    return RUN_TEST_SUITE(tests, elosLoggerInitializeUtest);
}

static int elosLoggerInitializeUtestSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosLoggerInitializeUtestTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
