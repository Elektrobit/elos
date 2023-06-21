// SPDX-License-Identifier: MIT
#include "safuJsonAddNewUint64_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewUint64SuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewUint64SuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewUint64ErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewUint64ErrNewObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewUint64ErrAddObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewUint64ErrAddObjectFailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
