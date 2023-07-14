// SPDX-License-Identifier: MIT
#include "elosRpnFilterInit_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterInitSuccess),
        TEST_CASE(elos_testElosRpnFilterInitErrParamNull),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
