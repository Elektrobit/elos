// SPDX-License-Identifier: MIT
#include "elosEventProcessorDeleteMembers_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventProcessorDeleteMembersErrComponent),
        TEST_CASE(elosTestEloEventProcessorDeleteMembersNullParam),
        TEST_CASE(elosTestEloEventProcessorDeleteMembersSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosTestState_t));
    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}
