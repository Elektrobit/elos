// SPDX-License-Identifier: MIT
#include "safuVecSet_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecSetSuccess),
                                       cmocka_unit_test(safuTestSafuVecSetErrOutOfBounds),
                                       cmocka_unit_test(safuTestSafuVecSetErrUninitialized)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
