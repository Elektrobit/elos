// SPDX-License-Identifier: MIT
#include "elosProcessFilterExecute_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloProcessFilterExecuteErrElement),
        TEST_CASE(elosTestEloProcessFilterExecuteErrParam),
        TEST_CASE(elosTestEloProcessFilterExecuteSuccessElement),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
