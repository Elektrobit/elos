// SPDX-License-Identifier: MIT
#include "elosEventInitialize_utest.h"

int elosTestElosEventInitializeSuccessSetup(void **state) {
    static elosEvent_t *testEvent = NULL;
    safuResultE_t retval = elosEventNew(&testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    testEvent->classification = 42;
    *state = testEvent;
    return 0;
}

int elosTestElosEventInitializeSuccessTeardown(void **state) {
    elosEvent_t *testEvent = *(elosEvent_t **)state;
    elosEventDelete(testEvent);
    return 0;
}

void elosTestElosEventInitializeSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEvent_t *testEvent = *(elosEvent_t **)state;

    TEST("elosEventInitialize");
    SHOULD("%s", "initialize the event to zero");

    assert_int_equal(testEvent->classification, 42);
    result = elosEventInitialize(testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testEvent->date.tv_sec, 0);
    assert_int_equal(testEvent->date.tv_nsec, 0);
    assert_null(testEvent->source.appName);
    assert_null(testEvent->source.fileName);
    assert_int_equal(testEvent->source.pid, 0);
    assert_int_equal(testEvent->severity, 0);
    assert_null(testEvent->hardwareid);
    assert_int_equal(testEvent->classification, 0);
    assert_int_equal(testEvent->messageCode, 0);
    assert_null(testEvent->payload);
}
