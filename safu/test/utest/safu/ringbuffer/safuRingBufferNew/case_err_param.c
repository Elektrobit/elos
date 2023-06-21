// SPDX-License-Identifier: MIT

#include "safuRingBufferNew_utest.h"

int safuTestSafuRingBufferNewErrParamSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferNewErrParamTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuRingBufferNewErrParam(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBuffer_t invalidRingBuffer = {0};
    safuRingBuffer_t *invalidRingBufferPtr = &invalidRingBuffer;
    safuResultE_t result;

    TEST("safuRingBufferNew");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = safuRingBufferNew(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBuffer, NULL");
    result = safuRingBufferNew(&invalidRingBufferPtr, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "ringBuffer, NULL");
    result = safuRingBufferNew(&test->ringBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
