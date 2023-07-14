// SPDX-License-Identifier: MIT
#include "elosEventIsDateEmpty_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(elosTestElosEventIsDateEmptyIsEmpty),
        cmocka_unit_test(elosTestElosEventIsDateEmptyIsNotEmpty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
