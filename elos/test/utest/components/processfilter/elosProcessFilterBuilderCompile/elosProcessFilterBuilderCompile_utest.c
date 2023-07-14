// SPDX-License-Identifier: MIT
#include "elosProcessFilterBuilderCompile_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloProcessFilterBuilderCompileErrParam),
        TEST_CASE(elosTestEloProcessFilterBuilderCompileSuccessLocation),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
