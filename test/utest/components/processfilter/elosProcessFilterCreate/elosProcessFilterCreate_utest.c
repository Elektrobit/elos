// SPDX-License-Identifier: MIT
#include "elosProcessFilterCreate_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloProcessFilterCreateErrParam),
        TEST_CASE(elosTestEloProcessFilterCreateSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
