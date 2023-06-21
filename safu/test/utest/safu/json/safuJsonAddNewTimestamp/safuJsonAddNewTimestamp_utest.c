// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewTimestampSuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampSuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrNewArrayArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrNewArrayObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrAddArrayArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrAddArrayObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrCreateSecArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrCreateSecObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrCreateNSecArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrCreateNSecObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrInsertSecArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrInsertSecObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrInsertNSecArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrInsertNSecObject),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrNoTimespecArray),
        TEST_CASE(safuTestSafuJsonAddNewTimestampErrNoTimespecObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
