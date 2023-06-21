// SPDX-License-Identifier: MIT
#include "safuVecFind_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecFindErrNoDataVec),
                                       cmocka_unit_test(safuTestSafuVecFindErrStartOutOfBounds),
                                       cmocka_unit_test(safuTestSafuVecFindErrNoResultMemory),
                                       cmocka_unit_test(safuTestSafuVecFindErrNoMatchFunc),
                                       cmocka_unit_test(safuTestSafuVecFindElementNotFound),
                                       cmocka_unit_test(safuTestSafuVecFindSuccess),
                                       cmocka_unit_test(safuTestSafuVecFindEmptyVec)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
