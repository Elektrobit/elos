// SPDX-License-Identifier: MIT

#include "elosEventSourceSerialize_utest.h"

int elosTestElosEventSourceSerializeExterrJsonObjStringSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceSerializeExterrJsonObjStringTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    free(test->receivedString);
    return 0;
}

void elosTestElosEventSourceSerializeExterrJsonObjString(void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t source = {.appName = "appNameTest", .fileName = "fileNameTest", .pid = 123};

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the string conversion from json object fails");

    MOCK_FUNC_AFTER_CALL(json_object_to_json_string_ext, 0);
    expect_not_value(__wrap_json_object_to_json_string_ext, obj, NULL);
    expect_value(__wrap_json_object_to_json_string_ext, flags, JSON_C_TO_STRING_PRETTY);
    will_return(__wrap_json_object_to_json_string_ext, NULL);

    result = elosEventSourceSerialize(&test->receivedString, &source);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
