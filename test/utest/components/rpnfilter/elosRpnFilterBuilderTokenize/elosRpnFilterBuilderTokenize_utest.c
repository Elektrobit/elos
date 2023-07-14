// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderTokenize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeErrInternal),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeErrParam),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeErrString),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeSuccessCommand),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeSuccessLocation),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeSuccessNumber),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeSuccessString),
        TEST_CASE(elos_testElosRpnFilterBuilderTokenizeSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
