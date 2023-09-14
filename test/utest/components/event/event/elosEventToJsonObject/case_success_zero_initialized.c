// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectSuccessZeroInitializedSetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.event);
    assert_int_equal(retval, SAFU_RESULT_OK);
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectSuccessZeroInitializedTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    elosEventDelete(test->event);
    return 0;
}

void elosTestElosEventToJsonObjectSuccessZeroInitialized(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {0};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "Convert the given zero initialized event to corresponding json object");

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);

    retval = elosEventFromJsonObject(test->event, test->result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(test->event->date.tv_sec, testEvent.date.tv_sec);
    assert_int_equal(test->event->date.tv_nsec, testEvent.date.tv_nsec);
    assert_ptr_equal(test->event->source.appName, testEvent.source.appName);
    assert_ptr_equal(test->event->source.fileName, testEvent.source.fileName);
    assert_int_equal(test->event->source.pid, testEvent.source.pid);
    assert_int_equal(test->event->severity, testEvent.severity);
    assert_int_equal(test->event->hardwareid, testEvent.hardwareid);
    assert_int_equal(test->event->classification, testEvent.classification);
    assert_int_equal(test->event->messageCode, testEvent.messageCode);
    assert_int_equal(test->event->payload, testEvent.payload);
}
