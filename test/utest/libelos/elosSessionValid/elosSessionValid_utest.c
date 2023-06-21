// SPDX-License-Identifier: MIT
#include "elosSessionValid_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosSessionValidSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    test->session.connected = true;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}