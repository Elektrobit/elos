// SPDX-License-Identifier: MIT

#include "safuRingBufferRead_utest.h"

#define _TEST_ELEMENTS            5
#define _TEST_OVERFLOW_ITERATIONS ((_TEST_ELEMENTS * 2) + 2)

int safuTestSafuRingBufferReadSuccessSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferReadSuccessTeardown(UNUSED void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);
    safuVecFree(&test->vector);

    return 0;
}

void safuTestSafuRingBufferReadSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    size_t testValue[_TEST_OVERFLOW_ITERATIONS] = {0};
    size_t testElementsWritten = 0;
    size_t **element;
    safuResultE_t result;

    for (size_t i = 0; i < _TEST_OVERFLOW_ITERATIONS; i += 1) {
        testValue[i] = i + 1;
    }

    TEST("safuRingBufferRead");
    SHOULD("%s", "test correct behaviour of safuRingBufferRead");

    PARAM("%s", "Read after one written Entry");
    result = safuRingBufferWrite(&test->ringBuffer, &testValue[0]);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = safuRingBufferRead(&test->ringBuffer, &test->vector, NULL);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->ringBuffer.nextIdx, 0);
    assert_int_equal(test->ringBuffer.elementsWritten, 0);
    assert_non_null(test->ringBuffer.data);
    assert_non_null(test->vector.data);
    assert_ptr_not_equal(test->vector.data, test->ringBuffer.data);
    assert_int_equal(test->vector.memorySize, test->ringBuffer.memorySize);
    assert_int_equal(test->vector.elementCount, 1);
    assert_int_equal(test->vector.elementSize, SAFU_RINGBUFFER_ELEMENT_SIZE);
    element = safuVecGetLast(&test->vector);
    assert_non_null(element);
    assert_int_equal(**element, testValue[0]);

    PARAM("%s", "Read after two written Entries");
    result = safuRingBufferWrite(&test->ringBuffer, &testValue[1]);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = safuRingBufferRead(&test->ringBuffer, &test->vector, NULL);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->vector.elementCount, 1);
    element = safuVecGetLast(&test->vector);
    assert_non_null(element);
    assert_int_equal(**element, testValue[1]);

    PARAM("%s", "Read after overflow");
    for (size_t i = 0; i < _TEST_OVERFLOW_ITERATIONS; i += 1) {
        result = safuRingBufferWrite(&test->ringBuffer, &testValue[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    result = safuRingBufferRead(&test->ringBuffer, &test->vector, &testElementsWritten);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testElementsWritten, _TEST_OVERFLOW_ITERATIONS);

    for (size_t i = 0; i < _TEST_ELEMENTS; i += 1) {
        size_t const valueIdx = (_TEST_OVERFLOW_ITERATIONS - _TEST_ELEMENTS + i);

        element = safuVecGet(&test->vector, i);
        assert_non_null(element);
        assert_int_equal(**element, testValue[valueIdx]);
    }
}
