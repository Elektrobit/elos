// SPDX-License-Identifier: MIT
#include "safuVecCreate_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecCreateSuccess),
        cmocka_unit_test(safuTestSafuVecCreateExterrMalloc),
        cmocka_unit_test(safuTestSafuVecCreateWrongParameter),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
