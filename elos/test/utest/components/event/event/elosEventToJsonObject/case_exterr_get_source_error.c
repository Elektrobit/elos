// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"
#include "mock_event_source.h"

int elosTestElosEventToJsonObjectSourceErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectSourceErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectSourceError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t testSource = {.pid = 234, .appName = "app234", .fileName = "/usr/local/app234"};
    const elosEvent_t testEvent = {.source = testSource};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new source object fails");

    MOCK_FUNC_AFTER_CALL(elosEventSourceToJsonObject, 0);
    expect_not_value(elosEventSourceToJsonObject, to, NULL);
    expect_not_value(elosEventSourceToJsonObject, from, NULL);
    will_return(elosEventSourceToJsonObject, SAFU_RESULT_FAILED);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
