// SPDX-License-Identifier: MIT
#include "elosEventFilterCreate_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterCreateErrParam),
        TEST_CASE(elosTestEloEventFilterCreateSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
