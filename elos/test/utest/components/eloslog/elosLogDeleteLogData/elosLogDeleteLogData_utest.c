// SPDX-License-Identifier: MIT
#include "elosLogDeleteLogData_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLogDeleteLogDataUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosLogDeleteLogDataSuccess),
    };
    return RUN_TEST_SUITE(tests, elosLogDeleteLogDataUtest);
}

static int elosLogDeleteLogDataUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLogDeleteLogDataUtestTeardown(UNUSED void **state) {
    return 0;
}
