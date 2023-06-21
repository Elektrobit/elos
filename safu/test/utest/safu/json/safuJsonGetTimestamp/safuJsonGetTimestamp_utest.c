// SPDX-License-Identifier: MIT
#include "safuJsonGetTimestamp_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetTimestampSuccessArray),
        TEST_CASE(safuTestSafuJsonGetTimestampSuccessObject),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetArrayFailedArray),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetArrayFailedObject),
        TEST_CASE(safuTestSafuJsonGetTimestampErrNoFullTimestampArray),
        TEST_CASE(safuTestSafuJsonGetTimestampErrNoFullTimestampObject),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetSecsFailedArray),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetSecsFailedObject),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetNSecsFailedArray),
        TEST_CASE(safuTestSafuJsonGetTimestampErrGetNSecsFailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
