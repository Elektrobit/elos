// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elosEventBufferRead_utest.h"

#define _TEST_EVENT_LIMIT          8
#define _TEST_EVENT_PAYLOAD_HIGH   "eventPriorityHigh"
#define _TEST_EVENT_PAYLOAD_NORMAL "eventPriorityNormal"

static int _deleteEventPtr(void const *element, UNUSED void const *data) {
    elosEventDelete(*(elosEvent_t **)element);
    return 0;
}

int elosTestElosEventBufferReadSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENT_LIMIT};
    elosEvent_t eventPriorityNormal = {.payload = _TEST_EVENT_PAYLOAD_NORMAL, .severity = ELOS_SEVERITY_INFO};
    elosEvent_t eventPriorityHigh = {.payload = _TEST_EVENT_PAYLOAD_HIGH, .severity = ELOS_SEVERITY_FATAL};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityNormal);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityHigh);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventPriorityHigh.messageCode = 1;
    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityHigh);
    assert_int_equal(result, SAFU_RESULT_OK);
    eventPriorityHigh.messageCode = 2;
    result = elosEventBufferWrite(&test->eventBuffer, &eventPriorityHigh);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferReadSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);
    safuVecIterate(&test->eventPtrVector, _deleteEventPtr, NULL);
    safuVecFree(&test->eventPtrVector);

    return 0;
}

void elosTestElosEventBufferReadSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    size_t elementsWritten = 0;
    size_t priority;
    elosEvent_t **eventPtr;
    safuResultE_t result;

    TEST("elosEventBufferRead");
    SHOULD("%s", "test correct behaviour of elosEventBufferRead");

    PARAM("%s", "read normal priority Event");
    priority = ELOS_EVENTBUFFER_PRIORITY_NORMAL;
    result = elosEventBufferRead(&test->eventBuffer, priority, &test->eventPtrVector, &elementsWritten);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(elementsWritten, 1);
    eventPtr = safuVecGetLast(&test->eventPtrVector);
    assert_ptr_not_equal(eventPtr, NULL);
    assert_ptr_not_equal(*eventPtr, NULL);
    assert_string_equal((*eventPtr)->payload, _TEST_EVENT_PAYLOAD_NORMAL);

    safuVecIterate(&test->eventPtrVector, _deleteEventPtr, NULL);
    safuVecFree(&test->eventPtrVector);

    PARAM("%s", "read high priority Event");
    priority = ELOS_EVENTBUFFER_PRIORITY_HIGH;
    result = elosEventBufferRead(&test->eventBuffer, priority, &test->eventPtrVector, &elementsWritten);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(elementsWritten, 3);
    eventPtr = safuVecGetLast(&test->eventPtrVector);
    assert_ptr_not_equal(eventPtr, NULL);
    assert_ptr_not_equal(*eventPtr, NULL);
    assert_string_equal((*eventPtr)->payload, _TEST_EVENT_PAYLOAD_HIGH);
    assert_int_equal((*eventPtr)->messageCode, 2);
}
