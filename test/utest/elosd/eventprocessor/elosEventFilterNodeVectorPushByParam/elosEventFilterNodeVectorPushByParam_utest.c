// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeVectorPushByParam_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeVectorPushByParamInitializeFailed),
        TEST_CASE(elosTestEloEventFilterNodeVectorPushByParamParamNull),
        TEST_CASE(elosTestEloEventFilterNodeVectorPushByParamPushFailed),
        TEST_CASE(elosTestEloEventFilterNodeVectorPushByParamSuccess),
        TEST_CASE(elosTestEloEventFilterNodeVectorPushByParamVectorNull),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
