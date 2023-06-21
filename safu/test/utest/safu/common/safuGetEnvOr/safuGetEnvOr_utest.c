// SPDX-License-Identifier: MIT
#include "safuGetEnvOr_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuGetEnvOrErrKeyIsNull),
        TEST_CASE(safuTestSafuGetEnvOrSuccessKeyExists),
        TEST_CASE(safuTestSafuGetEnvOrSuccessKeyNotExists),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
