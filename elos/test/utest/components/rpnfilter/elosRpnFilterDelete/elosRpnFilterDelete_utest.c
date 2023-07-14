// SPDX-License-Identifier: MIT
#include "elosRpnFilterDelete_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterDeleteErrParamNull),
        TEST_CASE(elos_testElosRpnFilterDeleteSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
