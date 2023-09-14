// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectSourceAddObjectErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectSourceAddObjectErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectSourceAddObjectError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t testSource = {.pid = 234, .appName = "app234", .fileName = "/usr/local/app234"};
    const elosEvent_t testEvent = {.source = testSource};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new object fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddObject, 0);
    expect_not_value(__wrap_safuJsonAddObject, jobj, NULL);
    expect_string(__wrap_safuJsonAddObject, name, "source");
    expect_not_value(__wrap_safuJsonAddObject, jdata, NULL);
    will_return(__wrap_safuJsonAddObject, -1);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
