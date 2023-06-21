// SPDX-License-Identifier: MIT
#include "safuVecGet_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecGetSuccess),
                                       cmocka_unit_test(safuTestSafuVecGetErrOutOfBounds),
                                       cmocka_unit_test(safuTestSafuVecGetErrUninitialized)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
