// SPDX-License-Identifier: MIT

#include "safuRingBufferRead_utest.h"

#define _TEST_ELEMENTS 42

int safuTestSafuRingBufferReadErrParamSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferReadErrParamTeardown(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferReadErrParam(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBuffer_t invalidRingBuffer = {0};
    safuResultE_t result;

    TEST("safuRingBufferRead");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = safuRingBufferRead(NULL, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBuffer, NULL");
    result = safuRingBufferRead(&invalidRingBuffer, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "RingBuffer, NULL");
    result = safuRingBufferRead(&test->ringBuffer, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
