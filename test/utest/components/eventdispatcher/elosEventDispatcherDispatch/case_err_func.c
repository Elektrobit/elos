// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_vector.h>
#include <signal.h>

#include "elosEventDispatcherDispatch_utest.h"

static void _fillEventBuffer(elosUnitTestState_t *test) {
    safuResultE_t result;
    size_t idx = 0;

    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_HIGH]);
    assert_int_equal(result, SAFU_RESULT_OK);
}

int testElosEventDispatcherDispatchErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;
    size_t idx = 0;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[idx]);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherDispatchErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherDispatchErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t *invalidEvent = NULL;
    safuResultE_t result;

    TEST("elosEventDispatcherDispatch");
    SHOULD("test behaviour when internal functions fail");

    PARAM("EventProcessorPublish fails");

    _fillEventBuffer(test);
    MOCK_FUNC_AFTER_CALL(elosEventProcessorPublish, 0);
    expect_not_value(elosEventProcessorPublish, eventProcessor, NULL);
    expect_not_value(elosEventProcessorPublish, event, NULL);
    will_return(elosEventProcessorPublish, SAFU_RESULT_FAILED);
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("Reading from EventBuffer fails");

    MOCK_FUNC_AFTER_CALL(elosEventBufferRead, 0);
    expect_not_value(elosEventBufferRead, eventBuffer, NULL);
    expect_value(elosEventBufferRead, priority, ELOS_EVENTBUFFER_PRIORITY_HIGH);
    expect_not_value(elosEventBufferRead, eventPtrVector, NULL);
    expect_not_value(elosEventBufferRead, elementsWritten, NULL);
    will_return(elosEventBufferRead, SAFU_RESULT_FAILED);

    _fillEventBuffer(test);
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("safuVecGet for EventPtrVector fails");

    MOCK_FUNC_AFTER_CALL(safuVecGet, 0);
    expect_not_value(__wrap_safuVecGet, vec, NULL);
    expect_value(__wrap_safuVecGet, idx, 0);
    will_return(__wrap_safuVecGet, NULL);

    _fillEventBuffer(test);
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("safuVecGet for EventPtrVector returns broken value");

    MOCK_FUNC_AFTER_CALL(safuVecGet, 0);
    expect_not_value(__wrap_safuVecGet, vec, NULL);
    expect_value(__wrap_safuVecGet, idx, 0);
    will_return(__wrap_safuVecGet, &invalidEvent);

    _fillEventBuffer(test);
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
