// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorDeleteMembers_utest.h"

int elosTestEloEventFilterNodeVectorDeleteMembersNullParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeVectorDeleteMembersNullParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeVectorDeleteMembersNullParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorDeleteMembers when given a null parameter");

    result = elosEventFilterNodeVectorDeleteMembers(NULL);
    assert_true(result == SAFU_RESULT_OK);
}
