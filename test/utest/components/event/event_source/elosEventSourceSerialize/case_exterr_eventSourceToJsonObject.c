// SPDX-License-Identifier: MIT

#include "elosEventSourceSerialize_utest.h"
#include "mock_event_source.h"

int elosTestElosEventSourceSerializeExterrEventSourceToJsonObjectSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceSerializeExterrEventSourceToJsonObjectTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    free(test->receivedString);
    return 0;
}

void elosTestElosEventSourceSerializeExterrEventSourceToJsonObject(void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t source = {.appName = "appNameTest", .fileName = "fileNameTest", .pid = 123};

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as conversion of event source to json object fails");

    MOCK_FUNC_AFTER_CALL(elosEventSourceToJsonObject, 0);
    expect_not_value(elosEventSourceToJsonObject, to, NULL);
    expect_not_value(elosEventSourceToJsonObject, from, NULL);
    will_return(elosEventSourceToJsonObject, SAFU_RESULT_FAILED);

    result = elosEventSourceSerialize(&test->receivedString, &source);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
