// SPDX-License-Identifier: MIT
#include "elosGetVersionString_utest.h"

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {
        cmocka_unit_test(elosTestElosetVersionStringFunctionCall),
    };

    return cmocka_run_group_tests(unitTests, NULL, NULL);
}
