// SPDX-License-Identifier: MIT
#include "safuJsonGetInt64_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetInt64SuccessArray),
        TEST_CASE(safuTestSafuJsonGetInt64SuccessObject),
        TEST_CASE(safuTestSafuJsonGetInt64ErrGetObjectFailedArray),
        TEST_CASE(safuTestSafuJsonGetInt64ErrGetObjectFailedObject),
        TEST_CASE(safuTestSafuJsonGetInt64ErrGetInt64FailedArray),
        TEST_CASE(safuTestSafuJsonGetInt64ErrGetInt64FailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
