// SPDX-License-Identifier: MIT

#include "safuRingBufferNew_utest.h"

#define _TEST_ELEMENTS 5

int safuTestSafuRingBufferNewSuccessSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferNewSuccessTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuRingBufferNewSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t param = {.elements = _TEST_ELEMENTS, .deleteEntries = false, .deleteFunc = NULL};
    safuResultE_t result;

    TEST("safuRingBufferNew");
    SHOULD("%s", "test correct behaviour of safuRingBufferNew");

    result = safuRingBufferNew(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->ringBuffer);
    // We only test one variable here to verify that safuRingBufferInitialize was called
    // A more comprehensive test of the initialized struct is done in the test for safuRingBufferInitialize
    assert_int_equal(test->ringBuffer->elements, _TEST_ELEMENTS);

    result = safuRingBufferDelete(&test->ringBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
}
