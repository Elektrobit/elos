// SPDX-License-Identifier: MIT
#include "safuJsonAddNewInt_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewIntSuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewIntSuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewIntErrAddObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewIntErrAddObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewIntErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewIntErrNewObjectFailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
