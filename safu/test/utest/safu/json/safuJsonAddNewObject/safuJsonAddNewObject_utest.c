// SPDX-License-Identifier: MIT
#include "safuJsonAddNewObject_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewObjectSuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewObjectSuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewObjectErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewObjectErrNewObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewObjectErrArray),
        TEST_CASE(safuTestSafuJsonAddNewObjectErrObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
