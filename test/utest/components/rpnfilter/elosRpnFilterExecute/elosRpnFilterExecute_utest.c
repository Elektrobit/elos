// SPDX-License-Identifier: MIT
#include "elosRpnFilterExecute_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterExecuteErrParam),
        TEST_CASE(elos_testElosRpnFilterExecuteSuccessArithmetic),
        TEST_CASE(elos_testElosRpnFilterExecuteSuccessOperation),
        TEST_CASE(elos_testElosRpnFilterExecuteSuccessString),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
