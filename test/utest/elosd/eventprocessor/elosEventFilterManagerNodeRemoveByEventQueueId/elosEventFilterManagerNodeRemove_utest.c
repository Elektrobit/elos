// SPDX-License-Identifier: MIT
#include "elosEventFilterManagerNodeRemove_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrParam),
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrRemove),
        TEST_CASE(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(*state);
    memset(*state, 0, sizeof(elosUnitTestState_t));
    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}
