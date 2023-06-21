// SPDX-License-Identifier: MIT
#include "safuJsonAddNewInt64_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewInt64ErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewInt64ErrNewObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewInt64ErrAddObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewInt64ErrAddObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewInt64SuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewInt64SuccessObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
