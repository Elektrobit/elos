// SPDX-License-Identifier: MIT

#include "safuRingBufferDeleteMembers_utest.h"

#define _TEST_ELEMENTS 5

typedef struct _dummyData {
    void *a;
    void *b;
} _dummyData_t;

static safuResultE_t _dummyDeleteFunc(void *data) {
    _dummyData_t *dummyData = (_dummyData_t *)data;

    free(dummyData->a);
    dummyData->a = NULL;

    free(dummyData->b);
    dummyData->b = NULL;

    return SAFU_RESULT_OK;
}

int safuTestSafuRingBufferDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuRingBufferDeleteMembersSuccessTeardown(UNUSED void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferDeleteMembersSuccess(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t paramA = {.elements = _TEST_ELEMENTS, .deleteEntries = false, .deleteFunc = NULL};
    safuRingBufferParam_t paramB = {.elements = _TEST_ELEMENTS, .deleteEntries = true, .deleteFunc = NULL};
    safuRingBufferParam_t paramC = {.elements = _TEST_ELEMENTS, .deleteEntries = true, .deleteFunc = _dummyDeleteFunc};
    _dummyData_t dummyData = {0};
    safuResultE_t result;
    void *element;

    TEST("safuRingBufferDeleteMembers");
    SHOULD("%s", "test correct behaviour of safuRingBufferDeleteMembers");

    PARAM("%s", "without deleteEntries");
    result = safuRingBufferInitialize(&test->ringBuffer, &paramA);
    assert_int_equal(result, SAFU_RESULT_OK);
    result = safuRingBufferDeleteMembers(&test->ringBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->ringBuffer.callback.delete, NULL);
    assert_ptr_equal(test->ringBuffer.data, NULL);
    assert_int_equal(test->ringBuffer.elements, 0);
    assert_int_equal(test->ringBuffer.elementsWritten, 0);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->ringBuffer.flags), false);
    assert_int_equal(SAFU_FLAG_HAS_DELETE_ENTRIES_BIT(&test->ringBuffer.flags), false);
    assert_int_equal(test->ringBuffer.memorySize, 0);
    assert_int_equal(test->ringBuffer.nextIdx, 0);

    PARAM("%s", "with deleteEntries and no deleteFunc");
    result = safuRingBufferInitialize(&test->ringBuffer, &paramB);
    assert_int_equal(result, SAFU_RESULT_OK);

    element = calloc(1, sizeof(size_t));
    assert_non_null(element);
    result = safuRingBufferWrite(&test->ringBuffer, element);

    result = safuRingBufferDeleteMembers(&test->ringBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "with deleteEntries and with deleteFunc");
    result = safuRingBufferInitialize(&test->ringBuffer, &paramC);
    assert_int_equal(result, SAFU_RESULT_OK);

    dummyData.a = calloc(1, sizeof(void *));
    assert_non_null(dummyData.a);
    dummyData.b = calloc(1, sizeof(void *));
    assert_non_null(dummyData.b);
    result = safuRingBufferWrite(&test->ringBuffer, &dummyData);

    result = safuRingBufferDeleteMembers(&test->ringBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(dummyData.a);
    assert_null(dummyData.b);
}
