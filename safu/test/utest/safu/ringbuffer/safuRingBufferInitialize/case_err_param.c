// SPDX-License-Identifier: MIT

#include "safuRingBufferInitialize_utest.h"

#define _TEST_ELEMENTS 42

int safuTestSafuRingBufferInitializeErrParamSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferInitializeErrParamTeardown(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferInitializeErrParam(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBuffer_t invalidRingBuffer = {0};
    safuResultE_t result;

    TEST("safuRingBufferInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = safuRingBufferInitialize(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidRingBuffer, NULL");
    result = safuRingBufferInitialize(&invalidRingBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "RingBuffer, NULL");
    result = safuRingBufferInitialize(&test->ringBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
