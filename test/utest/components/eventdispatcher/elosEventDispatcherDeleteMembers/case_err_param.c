// SPDX-License-Identifier: MIT

#include "elosEventDispatcherDeleteMembers_utest.h"

int testElosEventDispatcherDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherDeleteMembersErrParam(UNUSED void **state) {
    elosEventDispatcher_t invalidEventDispatcher = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventDispatcherDeleteMembers(NULL);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "invalidEventDispatcher");
    result = elosEventDispatcherDeleteMembers(&invalidEventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);
}
