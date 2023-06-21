// SPDX-License-Identifier: MIT
#include "safuJsonGetObject_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetObjectSuccessArray),
        TEST_CASE(safuTestSafuJsonGetObjectSuccessObject),
        TEST_CASE(safuTestSafuJsonGetObjectErrArray),
        TEST_CASE(safuTestSafuJsonGetObjectErrObject),
        TEST_CASE(safuTestSafuJsonGetObjectErrStringIndexArray),
        TEST_CASE(safuTestSafuJsonGetObjectErrNullKeyObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
