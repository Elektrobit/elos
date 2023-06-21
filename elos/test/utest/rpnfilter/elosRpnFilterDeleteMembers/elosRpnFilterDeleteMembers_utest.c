// SPDX-License-Identifier: MIT
#include "elosRpnFilterDeleteMembers_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterDeleteMembersErrParamNull),
        TEST_CASE(elos_testElosRpnFilterDeleteMembersSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
