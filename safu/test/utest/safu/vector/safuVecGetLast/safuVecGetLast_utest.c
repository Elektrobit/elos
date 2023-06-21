// SPDX-License-Identifier: MIT
#include "safuVecGetLast_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecGetLastSuccess),
                                       cmocka_unit_test(safuTestSafuVecGetLastErrEmpty),
                                       cmocka_unit_test(safuTestSafuVecGetLastErrUninitialized)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
