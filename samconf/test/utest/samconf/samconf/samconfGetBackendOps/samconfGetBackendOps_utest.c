// SPDX-License-Identifier: MIT
#include "samconfGetBackendOps_utest.h"

#include "samconf/samconf.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(samconfTestSamconfGetBackendOpsSuccessGetDummyBackend),
        cmocka_unit_test(samconfTestSamconfGetBackendOpsSuccessGetJsonBackend),
        cmocka_unit_test(samconfTestSamconfGetBackendOpsError),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
