// SPDX-License-Identifier: MIT
#include "elosEventProcessorInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventProcessorInitializeErrComponent),
        TEST_CASE(elosTestEloEventProcessorInitializeErrParam),
        TEST_CASE(elosTestEloEventProcessorInitializeSuccess),
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
