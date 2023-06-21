// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerDeleteMembers_utest.h"
#include "mock_eventfilternodevector.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterManagerDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterManagerDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterManagerDeleteMembersErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterManagerDeleteMembers");
    SHOULD("%s", "fail as the function is given an incorrect parameter");

    MOCK_FUNC_ALWAYS(elosEventFilterNodeVectorDeleteMembers);  // ensure function is NOT getting called

    result = elosEventFilterManagerDeleteMembers(NULL);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(elosEventFilterNodeVectorDeleteMembers);
}
