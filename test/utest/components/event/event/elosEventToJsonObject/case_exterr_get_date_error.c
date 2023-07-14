// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectDateErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectDateErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectDateError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {.date = {.tv_sec = 25, .tv_nsec = 100}};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new date object fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewTimestamp, 0);
    expect_not_value(__wrap_safuJsonAddNewTimestamp, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewTimestamp, name, "date");
    expect_value(__wrap_safuJsonAddNewTimestamp, timestamp, &testEvent.date);
    will_return(__wrap_safuJsonAddNewTimestamp, NULL);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
