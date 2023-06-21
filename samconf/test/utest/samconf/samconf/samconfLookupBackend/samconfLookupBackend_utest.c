// SPDX-License-Identifier: MIT
#include "samconfLookupBackend_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(samconfTestSamconfLookupBackendSuccess),
        cmocka_unit_test(samconfTestSamconfLookupBackendErrorNoLoop),
        cmocka_unit_test(samconfTestSamconfLookupBackendErrorSupportFail),
        cmocka_unit_test(samconfTestSamconfLookupBackendErrorAlloc),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
