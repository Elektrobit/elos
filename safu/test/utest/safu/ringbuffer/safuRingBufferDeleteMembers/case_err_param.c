// SPDX-License-Identifier: MIT

#include "safuRingBufferDeleteMembers_utest.h"

int safuTestSafuRingBufferDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuRingBufferDeleteMembersErrParam(UNUSED void **state) {
    safuRingBuffer_t invalidRingBuffer = {0};
    safuResultE_t result;

    TEST("safuRingBufferDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = safuRingBufferDeleteMembers(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBuffer");
    result = safuRingBufferDeleteMembers(&invalidRingBuffer);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
