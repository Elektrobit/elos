// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeVectorRemoveById_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeVectorRemoveByIdSuccess),
        TEST_CASE(elosTestEloEventFilterNodeVectorRemoveByIdVectorNull),
        TEST_CASE(elosTestEloEventFilterNodeVectorRemoveByIdInvalidId),
        TEST_CASE(elosTestEloEventFilterNodeVectorRemoveByIdFindFailed),
        TEST_CASE(elosTestEloEventFilterNodeVectorRemoveByIdRemoveFailed),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
