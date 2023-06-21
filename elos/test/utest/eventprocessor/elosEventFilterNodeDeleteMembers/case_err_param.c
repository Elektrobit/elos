// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeDeleteMembers_utest.h"

int elosTestEloEventFilterNodeDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeDeleteMembersErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterNodeDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    result = elosEventFilterNodeDeleteMembers(NULL);
    assert_true(result == SAFU_RESULT_OK);
}
