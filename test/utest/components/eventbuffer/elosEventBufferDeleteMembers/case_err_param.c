// SPDX-License-Identifier: MIT

#include "elosEventBufferDeleteMembers_utest.h"

#define _VECTORELEMENTS 1

int elosTestElosEventBufferDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferDeleteMembersErrParam(UNUSED void **state) {
    elosEventBuffer_t invalidEventBuffer = {0};
    safuResultE_t result;

    TEST("elosEventBufferDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventBufferDeleteMembers(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventBuffer");
    result = elosEventBufferDeleteMembers(&invalidEventBuffer);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
