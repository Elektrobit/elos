// SPDX-License-Identifier: MIT
#include "safuVecFree_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecFreeSuccess),
        cmocka_unit_test(safuTestSafuVecFreeUninitialized),
        cmocka_unit_test(safuTestSafuVecFreeErrNull),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
