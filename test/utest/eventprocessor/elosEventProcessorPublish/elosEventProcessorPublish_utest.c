// SPDX-License-Identifier: MIT
#include "elosEventProcessorPublish_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventProcessorPublishSuccess),
        TEST_CASE(elosTestEloEventProcessorPublishErrParam),
        TEST_CASE(elosTestEloEventProcessorPublishProcessFailed),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}
