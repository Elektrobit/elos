// SPDX-License-Identifier: MIT
#include "elosEventFilterBuilderCompile_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterBuilderCompileErrParam),
        TEST_CASE(elosTestEloEventFilterBuilderCompileSuccessLocation),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
