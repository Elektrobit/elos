// SPDX-License-Identifier: MIT
#include "safuStringIsEmpty_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuStringIsEmptySuccessStringIsEmpty),
        TEST_CASE(safuTestSafuStringIsEmptySuccessStringIsNotEmpty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
