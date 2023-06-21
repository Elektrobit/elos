// SPDX-License-Identifier: MIT
#include "safuGetHardwareId_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuGetHardwareIdEnv),
        TEST_CASE(safuTestSafuGetHardwareIdErrFile),
        // Must be run as the last test!
        TEST_CASE(safuTestSafuGetHardwareIdSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
