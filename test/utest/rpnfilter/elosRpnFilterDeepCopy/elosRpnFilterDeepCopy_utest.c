// SPDX-License-Identifier: MIT
#include "elosRpnFilterDeepCopy_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterDeepCopyErrParamNull),
        TEST_CASE(elos_testElosRpnFilterDeepCopySuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
