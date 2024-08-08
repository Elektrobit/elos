// SPDX-License-Identifier: MIT
#include "elosEventFilterExecute_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterExecuteErrElement),
        TEST_CASE(elosTestEloEventFilterExecuteErrParam),
        TEST_CASE(elosTestEloEventFilterExecuteSuccessElement),
        TEST_CASE(elosTestElosEventFilterExecuteSuccessHardwareid),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
