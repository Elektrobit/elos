// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectMessageCodeErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectMessageCodeErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectMessageCodeError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {.messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new messageCode object fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewInt, 0);
    expect_not_value(__wrap_safuJsonAddNewInt, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewInt, name, "messageCode");
    expect_value(__wrap_safuJsonAddNewInt, val, testEvent.messageCode);
    will_return(__wrap_safuJsonAddNewInt, NULL);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
