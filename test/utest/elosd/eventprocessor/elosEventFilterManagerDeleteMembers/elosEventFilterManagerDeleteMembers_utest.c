// SPDX-License-Identifier: MIT
#include "elosEventFilterManagerDeleteMembers_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterManagerDeleteMembersSuccess),
        TEST_CASE(elosTestEloEventFilterManagerDeleteMembersErrParam),
        TEST_CASE(elosTestEloEventFilterManagerDeleteMembersRemovalFailed),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
