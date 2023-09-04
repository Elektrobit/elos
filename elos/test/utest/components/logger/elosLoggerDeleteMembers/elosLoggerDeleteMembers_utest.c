// SPDX-License-Identifier: MIT

#include "elosLoggerDeleteMembers_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLoggerDeleteMembersUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoggerDeleteMembersSuccess),
        TEST_CASE(elosTestElosLoggerDeleteMembersErrFunc),
        TEST_CASE(elosTestElosLoggerDeleteMembersErrParam),
    };

    return RUN_TEST_SUITE(tests, elosLoggerDeleteMembersUtest);
}

static int elosLoggerDeleteMembersUtestSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosLoggerDeleteMembersUtestTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
