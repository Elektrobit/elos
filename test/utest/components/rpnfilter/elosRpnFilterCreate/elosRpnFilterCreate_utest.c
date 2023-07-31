// SPDX-License-Identifier: MIT
#include "elosRpnFilterCreate_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterCreateErrFilter),
        TEST_CASE(elos_testElosRpnFilterCreateErrParam),
        TEST_CASE(elos_testElosRpnFilterCreateSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
