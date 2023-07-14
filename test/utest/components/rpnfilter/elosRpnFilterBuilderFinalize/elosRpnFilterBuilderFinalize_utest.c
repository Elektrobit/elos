// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderFinalize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterBuilderFinalizeErrParam),
        TEST_CASE(elos_testElosRpnFilterBuilderFinalizeSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
