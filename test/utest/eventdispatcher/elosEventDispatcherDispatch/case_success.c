// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventDispatcherDispatch_utest.h"

static void _registerEventBuffers(elosUnitTestState_t *test) {
    safuResultE_t result;

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }
}

static void _fillEventBuffers(elosUnitTestState_t *test, elosEventCount_t *eventCount) {
    safuResultE_t result;
    size_t idx = 0;

    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;

    idx += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_HIGH]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->high += 1;

    idx += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_HIGH]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->high += 1;

    idx += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_NORMAL]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->normal += 1;
    result = elosEventBufferWrite(&test->eventBuffer[idx], &test->event[_EVENT_IDX_HIGH]);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventCount->high += 1;

    // One eventBuffer stays empty to have as many use cases as possible present
    idx += 1;

    assert_in_range(idx, 0, _EVENTBUFFER_ENTRIES - 1);
}

int testElosEventDispatcherDispatchSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    elosTestEventBufferReset(test);

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherDispatchSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherDispatchSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventCount_t eventCount = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherDispatch");
    SHOULD("test correct behaviour of elosEventDispatcherDispatch");

    PARAM("Test with no registered EventBuffers");

    elosEventProcessorPublishBandaid.result = SAFU_RESULT_FAILED;  // publish must not be called here
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("Test with no Events in the EventBuffers");

    _registerEventBuffers(test);
    elosEventProcessorPublishBandaid.result = SAFU_RESULT_FAILED;  // again, publish must not be called here
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("Test with Events in the EventBuffers");
    elosEventProcessorPublishBandaid.result = SAFU_RESULT_OK;
    elosEventProcessorPublishBandaid.eventCount.high = 0;
    elosEventProcessorPublishBandaid.eventCount.normal = 0;
    _fillEventBuffers(test, &eventCount);
    result = elosEventDispatcherDispatch(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(elosEventProcessorPublishBandaid.eventCount.high, eventCount.high);
    assert_int_equal(elosEventProcessorPublishBandaid.eventCount.normal, eventCount.normal);
}
