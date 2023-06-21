// SPDX-License-Identifier: MIT

#include "safuRingBufferInitialize_utest.h"

#define _TEST_ELEMENTS 5

// This function exists here only to have a pointer to pass around.
// It is properly tested in the test for safuRingBufferDeleteMembers.
static safuResultE_t _dummyDeleteFunc(UNUSED void *element) {
    return SAFU_RESULT_OK;
}

int safuTestSafuRingBufferInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferInitializeSuccessTeardown(UNUSED void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferInitializeSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t paramA = {.elements = _TEST_ELEMENTS, .deleteEntries = false, .deleteFunc = NULL};
    safuRingBufferParam_t paramB = {.elements = _TEST_ELEMENTS, .deleteEntries = true, .deleteFunc = _dummyDeleteFunc};
    safuResultE_t result;

    TEST("safuRingBufferInitialize");
    SHOULD("%s", "test correct behaviour of safuRingBufferInitialize");

    PARAM("%s", "Initialize without deleteFunc");
    result = safuRingBufferInitialize(&test->ringBuffer, &paramA);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->ringBuffer.callback.delete, NULL);
    assert_non_null(test->ringBuffer.data);
    assert_int_equal(test->ringBuffer.elements, _TEST_ELEMENTS);
    assert_int_equal(test->ringBuffer.elementsWritten, 0);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->ringBuffer.flags), true);
    assert_int_equal(SAFU_FLAG_HAS_DELETE_ENTRIES_BIT(&test->ringBuffer.flags), false);
    assert_int_equal(test->ringBuffer.memorySize, (_TEST_ELEMENTS * SAFU_RINGBUFFER_ELEMENT_SIZE));
    assert_int_equal(test->ringBuffer.nextIdx, 0);

    safuRingBufferDeleteMembers(&test->ringBuffer);

    PARAM("%s", "Initialize with deleteFunc");
    result = safuRingBufferInitialize(&test->ringBuffer, &paramB);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->ringBuffer.callback.delete, _dummyDeleteFunc);
    assert_non_null(test->ringBuffer.data);
    assert_int_equal(test->ringBuffer.elements, _TEST_ELEMENTS);
    assert_int_equal(test->ringBuffer.elementsWritten, 0);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->ringBuffer.flags), true);
    assert_int_equal(SAFU_FLAG_HAS_DELETE_ENTRIES_BIT(&test->ringBuffer.flags), true);
    assert_int_equal(test->ringBuffer.memorySize, (_TEST_ELEMENTS * SAFU_RINGBUFFER_ELEMENT_SIZE));
    assert_int_equal(test->ringBuffer.nextIdx, 0);
}
