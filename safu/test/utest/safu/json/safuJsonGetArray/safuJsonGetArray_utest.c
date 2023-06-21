// SPDX-License-Identifier: MIT
#include "safuJsonGetArray_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetArraySuccessArray),      TEST_CASE(safuTestSafuJsonGetArraySuccessObject),
        TEST_CASE(safuTestSafuJsonGetArraySuccessNoLenArray), TEST_CASE(safuTestSafuJsonGetArraySuccessNoLenObject),
        TEST_CASE(safuTestSafuJsonGetArrayErrNoArrayArray),   TEST_CASE(safuTestSafuJsonGetArrayErrNoArrayObject),
        TEST_CASE(safuTestSafuJsonGetArrayErrGetNullArray),   TEST_CASE(safuTestSafuJsonGetArrayErrGetNullObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
