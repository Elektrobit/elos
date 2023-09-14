// SPDX-License-Identifier: MIT
#include "elosEventDeleteMembers_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventDeleteMembersSuccess),
        TEST_CASE(elosTestElosEventDeleteMembersWrongParameter),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
