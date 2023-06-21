// SPDX-License-Identifier: MIT
#include "safuJsonAddNewArray_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddNewArraySuccessArray),
        TEST_CASE(safuTestSafuJsonAddNewArraySuccessObject),
        TEST_CASE(safuTestSafuJsonAddNewArrayErrAddObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewArrayErrAddObjectFailedObject),
        TEST_CASE(safuTestSafuJsonAddNewArrayErrNewObjectFailedArray),
        TEST_CASE(safuTestSafuJsonAddNewArrayErrNewObjectFailedObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
