// SPDX-License-Identifier: MIT

#include "safuRingBufferWrite_utest.h"

#define _TEST_ELEMENTS 42

int safuTestSafuRingBufferWriteErrParamSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferWriteErrParamTeardown(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferWriteErrParam(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBuffer_t invalidRingBuffer = {0};
    safuResultE_t result;

    TEST("safuRingBufferWrite");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = safuRingBufferWrite(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBuffer, NULL");
    result = safuRingBufferWrite(&invalidRingBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "RingBuffer, NULL");
    result = safuRingBufferWrite(&test->ringBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
