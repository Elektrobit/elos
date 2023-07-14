// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderDelete_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterBuilderDeleteErrParam),
        TEST_CASE(elos_testElosRpnFilterBuilderDeleteSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
