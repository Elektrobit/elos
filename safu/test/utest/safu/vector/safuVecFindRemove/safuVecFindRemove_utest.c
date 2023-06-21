// SPDX-License-Identifier: MIT
#include "safuVecFindRemove_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecFindRemoveErrUninitializedVec),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveErrNoMatchElement),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveErrNoMatchFunc),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveElementNotFound),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveSuccess),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveSuccessMultiMatch),
                                       cmocka_unit_test(safuTestSafuVecFindRemoveEmptyVec)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
