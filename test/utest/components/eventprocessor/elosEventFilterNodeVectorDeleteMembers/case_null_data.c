// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorDeleteMembers_utest.h"

int elosTestEloEventFilterNodeVectorDeleteMembersNullDataSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));

    test->efev.data = NULL;

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorDeleteMembersNullDataTeardown(void **state) {
    elosTestState_t *test = *state;

    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorDeleteMembersNullData(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorDeleteMembers when given a null parameter");

    result = elosEventFilterNodeVectorDeleteMembers(&test->efev);
    assert_true(result == SAFU_RESULT_OK);
}
