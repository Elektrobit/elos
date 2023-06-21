// SPDX-License-Identifier: MIT
#include "safuVecElements_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecElementsSuccess),
        cmocka_unit_test(safuTestSafuVecElementsErrUninitialized),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
