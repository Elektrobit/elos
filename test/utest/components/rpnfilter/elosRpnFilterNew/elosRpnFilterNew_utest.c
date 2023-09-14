// SPDX-License-Identifier: MIT
#include "elosRpnFilterNew_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterNewErrAlloc),
        TEST_CASE(elos_testElosRpnFilterNewErrInit),
        TEST_CASE(elos_testElosRpnFilterNewErrParamNull),
        TEST_CASE(elos_testElosRpnFilterNewSuccess),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
