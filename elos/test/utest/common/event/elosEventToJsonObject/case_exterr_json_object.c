// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectJsonObjectErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectJsonObjectErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectJsonObjectError(UNUSED void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {0};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as new json object creation fails");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
