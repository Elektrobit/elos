// SPDX-License-Identifier: MIT
#include "elosEventNew_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(elosTestElosEventNewSuccess),
        cmocka_unit_test(elosTestElosEventNewExterrAllocMem),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
