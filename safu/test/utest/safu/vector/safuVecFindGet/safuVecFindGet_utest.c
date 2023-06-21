// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecFindGetErrUninitializedVec),
                                       cmocka_unit_test(safuTestSafuVecFindGetErrStartOutOfBounds),
                                       cmocka_unit_test(safuTestSafuVecFindGetErrNoResultMemory),
                                       cmocka_unit_test(safuTestSafuVecFindGetErrNoMatchFunc),
                                       cmocka_unit_test(safuTestSafuVecFindGetErrNoMatchData),
                                       cmocka_unit_test(safuTestSafuVecFindGetElementNotFound),
                                       cmocka_unit_test(safuTestSafuVecFindGetSuccess),
                                       cmocka_unit_test(safuTestSafuVecFindGetEmptyVec)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
