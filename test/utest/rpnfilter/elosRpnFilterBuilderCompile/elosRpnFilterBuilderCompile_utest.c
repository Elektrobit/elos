// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderCompile_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterBuilderCompileErrParam),
        TEST_CASE(elos_testElosRpnFilterBuilderCompileSuccessCommand),
        TEST_CASE(elos_testElosRpnFilterBuilderCompileSuccessLocation),
        TEST_CASE(elos_testElosRpnFilterBuilderCompileSuccessString),
        TEST_CASE(elos_testElosRpnFilterBuilderCompileSuccessNumber),
        TEST_CASE(elos_testElosRpnFilterBuilderCompileSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
