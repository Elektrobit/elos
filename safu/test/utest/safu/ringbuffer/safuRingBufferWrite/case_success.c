// SPDX-License-Identifier: MIT

#include "safuRingBufferWrite_utest.h"

#define _TEST_ELEMENTS            5
#define _TEST_OVERFLOW_ITERATIONS ((_TEST_ELEMENTS * 2) + 1)

int safuTestSafuRingBufferWriteSuccessSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferWriteSuccessTeardown(UNUSED void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferWriteSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    size_t testValue[_TEST_OVERFLOW_ITERATIONS] = {0};
    safuResultE_t result;

    for (size_t i = 0; i < _TEST_OVERFLOW_ITERATIONS; i += 1) {
        testValue[i] = i + 1;
    }

    TEST("safuRingBufferWrite");
    SHOULD("%s", "test correct behaviour of safuRingBufferWrite");

    PARAM("%s", "write first Entry");
    result = safuRingBufferWrite(&test->ringBuffer, &testValue[0]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->ringBuffer.nextIdx, 1);
    assert_int_equal(*(size_t *)(test->ringBuffer.data[0]), testValue[0]);

    PARAM("%s", "write second Entry");
    result = safuRingBufferWrite(&test->ringBuffer, &testValue[1]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->ringBuffer.nextIdx, 2);
    assert_int_equal(*(size_t *)(test->ringBuffer.data[1]), testValue[1]);

    PARAM("%s", "write Entries until buffer overflow");
    for (size_t i = 2; i < _TEST_OVERFLOW_ITERATIONS; i += 1) {
        size_t idx = (i + 1) % _TEST_ELEMENTS;
        size_t dataIdx = i % _TEST_ELEMENTS;

        result = safuRingBufferWrite(&test->ringBuffer, &testValue[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
        assert_int_equal(test->ringBuffer.nextIdx, idx);
        assert_int_equal(*(size_t *)(test->ringBuffer.data[dataIdx]), testValue[i]);
    }

    size_t finalIdx = (_TEST_OVERFLOW_ITERATIONS % _TEST_ELEMENTS);
    size_t finalDataIdx = ((_TEST_OVERFLOW_ITERATIONS - 1) % _TEST_ELEMENTS);

    assert_int_equal(test->ringBuffer.nextIdx, finalIdx);
    assert_int_equal(*(size_t *)(test->ringBuffer.data[finalDataIdx]), testValue[_TEST_OVERFLOW_ITERATIONS - 1]);
}
