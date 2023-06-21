// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderNew_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterBuilderNewErrParam),
        TEST_CASE(elos_testElosRpnFilterBuilderNewSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
