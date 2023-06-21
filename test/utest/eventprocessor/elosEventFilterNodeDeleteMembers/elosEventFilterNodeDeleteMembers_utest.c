// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeDeleteMembers_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeDeleteMembersErrParam),
        TEST_CASE(elosTestEloEventFilterNodeDeleteMembersSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
