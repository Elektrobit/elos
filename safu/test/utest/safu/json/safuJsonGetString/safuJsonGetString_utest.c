// SPDX-License-Identifier: MIT
#include "safuJsonGetString_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetStringSuccessArray),
        TEST_CASE(safuTestSafuJsonGetStringSuccessObject),
        TEST_CASE(safuTestSafuJsonGetStringErrGetStringFailedArray),
        TEST_CASE(safuTestSafuJsonGetStringErrGetStringFailedObject),
        TEST_CASE(safuTestSafuJsonGetStringErrGetObjectFailedArray),
        TEST_CASE(safuTestSafuJsonGetStringErrGetObjectFailedObject),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
