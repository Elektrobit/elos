// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {
        cmocka_unit_test(elosTestElossVersionRequestedEmptyRequest),
        cmocka_unit_test(elosTestElossVersionRequestedLongRequest),
        cmocka_unit_test(elosTestElossVersionRequestedNoRequest),
        cmocka_unit_test(elosTestElossVersionRequestedNullParam),
        cmocka_unit_test(elosTestElossVersionRequestedShortRequest),
    };

    return cmocka_run_group_tests(unitTests, NULL, NULL);
}
