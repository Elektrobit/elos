// SPDX-License-Identifier: MIT
#include "safuJsonGetUint32_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetUint32SuccessArray),
        TEST_CASE(safuTestSafuJsonGetUint32SuccessObject),
        TEST_CASE(safuTestSafuJsonGetUint32NumberOutOfBoundsArray),
        TEST_CASE(safuTestSafuJsonGetUint32NumberOutOfBoundsObject),
        TEST_CASE(safuTestSafuJsonGetUint32ErrGetUint64FailedArray),
        TEST_CASE(safuTestSafuJsonGetUint32ErrGetUint64FailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
