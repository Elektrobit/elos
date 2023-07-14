// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectSeverityErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectSeverityErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectSeverityError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new severity object fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewInt, 0);
    expect_not_value(__wrap_safuJsonAddNewInt, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewInt, name, "severity");
    expect_value(__wrap_safuJsonAddNewInt, val, testEvent.severity);
    will_return(__wrap_safuJsonAddNewInt, NULL);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
