// SPDX-License-Identifier: MIT
#include "safuJsonGetInt32_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuJsonGetInt32SuccessArray),
        TEST_CASE(safuTestSafuJsonGetInt32SuccessObject),
        TEST_CASE(safuTestSafuJsonGetInt32ErrGetObjectFailedArray),
        TEST_CASE(safuTestSafuJsonGetInt32ErrGetObjectFailedObject),
        TEST_CASE(safuTestSafuJsonGetInt32ErrGetIntArray),
        TEST_CASE(safuTestSafuJsonGetInt32ErrGetIntObject),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
