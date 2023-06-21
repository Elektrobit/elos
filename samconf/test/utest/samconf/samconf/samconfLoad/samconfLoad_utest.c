// SPDX-License-Identifier: MIT
#include "samconfLoad_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(samconfTestSamconfLoadSuccess),
        cmocka_unit_test(samconfTestSamconfLoadOpenError),
        cmocka_unit_test(samconfTestSamconfLoadLoadError),
        cmocka_unit_test(samconfTestSamconfLoadCloseError),
        cmocka_unit_test(samconfTestSamconfLoadLookupError),
        cmocka_unit_test(samconfTestSamconfLoadBackendDeleteError),
        cmocka_unit_test(samconfTestSamconfLoadConfigDeleteError),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
