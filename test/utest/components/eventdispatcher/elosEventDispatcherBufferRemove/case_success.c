// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventDispatcherBufferRemove_utest.h"

#define _VALUE_FIRST  0
#define _VALUE_MIDDLE 1
#define _VALUE_LAST   2

static inline void _testEventBuffer(elosUnitTestState_t *test, uint32_t idx, uint32_t limitEventCount) {
    elosEventBuffer_t **eventBuffer;

    eventBuffer = safuVecGet(&test->eventDispatcher.eventBufferPtrVector, idx);
    assert_ptr_not_equal(eventBuffer, NULL);
    assert_ptr_not_equal(*eventBuffer, NULL);
    assert_int_equal((*eventBuffer)->ring[0].limitEventCount, limitEventCount);
}

int testElosEventDispatcherBufferRemoveSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    return 0;
}

int testElosEventDispatcherBufferRemoveSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferRemoveSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherBufferRemove");
    SHOULD("test correct behaviour of elosEventDispatcherBufferRemove");

    PARAM("Remove middle EventBuffer");
    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[_VALUE_MIDDLE]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&test->eventDispatcher.eventBufferPtrVector), 2);
    _testEventBuffer(test, 0, _VALUE_FIRST + 1);
    _testEventBuffer(test, 1, _VALUE_LAST + 1);

    PARAM("Remove first EventBuffer");
    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[_VALUE_FIRST]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&test->eventDispatcher.eventBufferPtrVector), 1);
    _testEventBuffer(test, 0, _VALUE_LAST + 1);

    PARAM("Remove last EventBuffer");
    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[_VALUE_LAST]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&test->eventDispatcher.eventBufferPtrVector), 0);
}
