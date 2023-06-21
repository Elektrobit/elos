// SPDX-License-Identifier: MIT

#include "safuRingBufferDelete_utest.h"

int safuTestSafuRingBufferDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuRingBufferDeleteErrParam(UNUSED void **state) {
    safuRingBuffer_t invalidRingBuffer = {0};
    safuRingBuffer_t *invalidRingBufferPtr = &invalidRingBuffer;
    safuResultE_t result;

    TEST("safuRingBufferDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = safuRingBufferDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBufferPtr");
    result = safuRingBufferDelete(&invalidRingBufferPtr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
