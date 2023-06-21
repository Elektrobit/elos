// SPDX-License-Identifier: MIT
#include "safuJsonAddNewString_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewStringSuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewStringSuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewStringErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewStringErrNewObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewStringErrAddObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewStringErrAddObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewStringErrNullStringArray),
        TEST_CASE(safuTestSafuJsonAddNewStringErrNullStringObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
