// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeVectorDeleteMembers_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeVectorDeleteMembersNullParam),
        TEST_CASE(elosTestEloEventFilterNodeVectorDeleteMembersNullData),
        TEST_CASE(elosTestEloEventFilterNodeVectorDeleteMembersFreeFailed),
        TEST_CASE(elosTestEloEventFilterNodeVectorDeleteMembersIterateFailed),
        TEST_CASE(elosTestEloEventFilterNodeVectorDeleteMembersSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
