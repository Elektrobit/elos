// SPDX-License-Identifier: MIT
#include "elosPrintVersion_utest.h"

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {
        cmocka_unit_test(elosTestElosrintVersionNullPrefix),
        cmocka_unit_test(elosTestElosrintVersionWithPrefix),
    };

    return cmocka_run_group_tests(unitTests, NULL, NULL);
}
