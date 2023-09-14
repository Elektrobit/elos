// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectSuccessValidTimeValuesSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectSuccessValidTimeValuesTeardown(UNUSED void **state) {
    return 0;
}

void _runElosEventToJsonObjectSuccessValidTimeValues(void **state, const elosEvent_t *testEvent) {
    elosTestState_t *test = *(elosTestState_t **)state;

    test->result = json_object_new_object();
    assert_non_null(test->result);

    safuResultE_t retval = elosEventNew(&test->event);
    assert_int_equal(retval, SAFU_RESULT_OK);

    retval = elosEventToJsonObject(test->result, testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);

    retval = elosEventFromJsonObject(test->event, test->result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(test->event->date.tv_sec, testEvent->date.tv_sec);
    assert_int_equal(test->event->date.tv_nsec, testEvent->date.tv_nsec);
    assert_ptr_equal(test->event->source.appName, testEvent->source.appName);
    assert_ptr_equal(test->event->source.fileName, testEvent->source.fileName);
    assert_int_equal(test->event->source.pid, testEvent->source.pid);
    assert_int_equal(test->event->severity, testEvent->severity);
    assert_int_equal(test->event->hardwareid, testEvent->hardwareid);
    assert_int_equal(test->event->classification, testEvent->classification);
    assert_int_equal(test->event->messageCode, testEvent->messageCode);
    assert_int_equal(test->event->payload, testEvent->payload);

    elosEventDelete(test->event);
    json_object_put(test->result);
}

void elosTestElosEventToJsonObjectSuccessValidTimeValues(void **state) {
    const elosEvent_t testEvent[] = {
        {.date = {.tv_sec = 0, .tv_nsec = 100}},
        {.date = {.tv_sec = 39, .tv_nsec = 0}},
        {.date = {.tv_sec = INT_MAX / 5, .tv_nsec = LONG_MAX / 2}},
        {.date = {.tv_sec = INT_MAX, .tv_nsec = LONG_MAX}},
    };

    TEST("elosEventToJsonObject");
    SHOULD("%s", "convert the given event with valid date values to corresponding json object");

    for (size_t i = 0; i < ARRAY_SIZE(testEvent); i++) {
        _runElosEventToJsonObjectSuccessValidTimeValues(state, &testEvent[i]);
    }
}
