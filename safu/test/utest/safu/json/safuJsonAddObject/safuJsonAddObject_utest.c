// SPDX-License-Identifier: MIT
#include "safuJsonAddObject_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonAddObjectArray),
        TEST_CASE(safuTestSafuJsonAddObjectObject),
        TEST_CASE(safuTestSafuJsonAddObjectErrArray),
        TEST_CASE(safuTestSafuJsonAddObjectErrObject),
        TEST_CASE(safuTestSafuJsonAddObjectErrNullKeyObject),
        TEST_CASE(safuTestSafuJsonAddObjectErrStringIndexArray),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
