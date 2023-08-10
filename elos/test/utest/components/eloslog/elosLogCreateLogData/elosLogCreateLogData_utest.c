// SPDX-License-Identifier: MIT
#include "elosLogCreateLogData_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLogCreateLogDataUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosLogCreateLogDataSuccess),
    };
    return RUN_TEST_SUITE(tests, elosLogCreateLogDataUtest);
}

static int elosLogCreateLogDataUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLogCreateLogDataUtestTeardown(UNUSED void **state) {
    return 0;
}
