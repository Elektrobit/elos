// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosEventBufferWrite_utest.h"

#define _TEST_WRITETRIGGER_VALUE   42
#define _TEST_EVENT_LIMIT          8
#define _TEST_EVENT_PAYLOAD_HIGH   "eventPriorityHigh"
#define _TEST_EVENT_PAYLOAD_NORMAL "eventPriorityNormal"

int elosTestElosEventBufferWriteSuccessTriggerSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENT_LIMIT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventBufferSetWriteTrigger(&test->eventBuffer, _TEST_WRITETRIGGER_VALUE);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferWriteSuccessTriggerTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferWriteSuccessTrigger(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t eventPriorityNormal = {.payload = _TEST_EVENT_PAYLOAD_NORMAL, .severity = ELOS_SEVERITY_INFO};
    elosEvent_t eventPriorityHigh = {.payload = _TEST_EVENT_PAYLOAD_HIGH, .severity = ELOS_SEVERITY_FATAL};
    elosEventBufferRing_t *ringPriorityHigh = &test->eventBuffer.ring[ELOS_EVENTBUFFER_PRIORITY_HIGH];
    elosEventBufferRing_t *ringPriorityNormal = &test->eventBuffer.ring[ELOS_EVENTBUFFER_PRIORITY_NORMAL];
    elosEvent_t *event;
    safuResultE_t result;

    TEST("elosEventBufferWrite");
    SHOULD("%s", "test correct behaviour of elosEventBufferWrite");

    PARAM("%s", "write normal priority Event with trigger");

    MOCK_FUNC_AFTER_CALL(eventfd_write, 0);
    expect_value(__wrap_eventfd_write, __fd, _TEST_WRITETRIGGER_VALUE);
    expect_value(__wrap_eventfd_write, __value, 1);
    will_return(__wrap_eventfd_write, 0);

    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityNormal);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(ringPriorityHigh->ringBuffer.elementsWritten, 0);
    assert_int_equal(ringPriorityHigh->ringBuffer.nextIdx, 0);
    assert_int_equal(ringPriorityNormal->ringBuffer.elementsWritten, 1);
    assert_int_equal(ringPriorityNormal->ringBuffer.nextIdx, 1);

    event = ringPriorityNormal->ringBuffer.data[0];
    assert_ptr_not_equal(event, NULL);
    assert_string_equal(event->payload, _TEST_EVENT_PAYLOAD_NORMAL);
    assert_int_equal(event->severity, ELOS_SEVERITY_INFO);

    PARAM("%s", "write high priority Event with trigger");

    MOCK_FUNC_AFTER_CALL(eventfd_write, 0);
    expect_value(__wrap_eventfd_write, __fd, _TEST_WRITETRIGGER_VALUE);
    expect_value(__wrap_eventfd_write, __value, 1);
    will_return(__wrap_eventfd_write, 0);

    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityHigh);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(ringPriorityHigh->ringBuffer.elementsWritten, 1);
    assert_int_equal(ringPriorityHigh->ringBuffer.nextIdx, 1);
    assert_int_equal(ringPriorityNormal->ringBuffer.elementsWritten, 1);
    assert_int_equal(ringPriorityNormal->ringBuffer.nextIdx, 1);

    event = ringPriorityHigh->ringBuffer.data[0];
    assert_ptr_not_equal(event, NULL);
    assert_string_equal(event->payload, _TEST_EVENT_PAYLOAD_HIGH);
    assert_int_equal(event->severity, ELOS_SEVERITY_FATAL);
}
