// SPDX-License-Identifier: MIT

#include "safuRingBufferDelete_utest.h"

#define _TEST_ELEMENTS 5

// This function exists here as a workaround to verify that safuRingBufferDeleteMembers was called.
// It is properly tested in the test for safuRingBufferDeleteMembers.
static safuResultE_t _dummyDeleteFunc(void *element) {
    size_t *value = (size_t *)element;
    *value = 42;
    return SAFU_RESULT_OK;
}

int safuTestSafuRingBufferDeleteSuccessSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t param = {.elements = _TEST_ELEMENTS, .deleteEntries = true, .deleteFunc = _dummyDeleteFunc};
    safuResultE_t result;

    result = safuRingBufferNew(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    // Write a dummy value to ensure our _dummyDeleteFunc gets called during the safuRingBufferDeleteMembers call
    test->value = 0;
    result = safuRingBufferWrite(test->ringBuffer, &test->value);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuRingBufferDeleteSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuResultE_t result;

    TEST("safuRingBufferDelete");
    SHOULD("%s", "test correct behaviour of safuRingBufferDelete");

    result = safuRingBufferDelete(&test->ringBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->value, 42);
    assert_ptr_equal(test->ringBuffer, NULL);
}
