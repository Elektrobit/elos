// SPDX-License-Identifier: MIT
#include "safuJsonGetUint64_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetUint64SuccessArray),
        TEST_CASE(safuTestSafuJsonGetUint64SuccessObject),
        TEST_CASE(safuTestSafuJsonGetUint64ErrGetObjectFailedArray),
        TEST_CASE(safuTestSafuJsonGetUint64ErrGetObjectFailedObject),
        TEST_CASE(safuTestSafuJsonGetUint64ErrGetUint64FailedArray),
        TEST_CASE(safuTestSafuJsonGetUint64ErrGetUint64FailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
