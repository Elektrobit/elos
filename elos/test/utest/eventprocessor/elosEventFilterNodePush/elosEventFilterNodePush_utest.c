// SPDX-License-Identifier: MIT
#include "elosEventFilterNodePush_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodePushCountZero),
        TEST_CASE(elosTestEloEventFilterNodePushCreateFailed),
        TEST_CASE(elosTestEloEventFilterNodePushEfeNull),
        TEST_CASE(elosTestEloEventFilterNodePushFilterNull),
        TEST_CASE(elosTestEloEventFilterNodePushFilterStringNull),
        TEST_CASE(elosTestEloEventFilterNodePushPushFailed),
        TEST_CASE(elosTestEloEventFilterNodePushSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
