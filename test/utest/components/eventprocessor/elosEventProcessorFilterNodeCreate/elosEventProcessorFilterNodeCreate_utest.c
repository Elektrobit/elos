// SPDX-License-Identifier: MIT
#include "elosEventProcessorFilterNodeCreate_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventProcessorFilterNodeCreateSuccess),
        TEST_CASE(elosTestEloEventProcessorFilterNodeCreateErrParam),
        TEST_CASE(elosTestEloEventProcessorFilterNodeCreateCreateFailed),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(UNUSED void **state) {
    return 0;
}

static int _testSuiteTeardown(UNUSED void **state) {
    return 0;
}
