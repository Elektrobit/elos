// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectClassificationErrorSetup(void **state) {
    static elosTestState_t test = {0};
    test.result = json_object_new_object();
    assert_non_null(test.result);
    *state = &test;
    return 0;
}

int elosTestElosEventToJsonObjectClassificationErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->result);
    return 0;
}

void elosTestElosEventToJsonObjectClassificationError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosEvent_t testEvent = {.classification = 42};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return null as addition of new classification object fails");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewUint64, 0);
    expect_not_value(__wrap_safuJsonAddNewUint64, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewUint64, name, "classification");
    expect_value(__wrap_safuJsonAddNewUint64, val, testEvent.classification);
    will_return(__wrap_safuJsonAddNewUint64, NULL);

    retval = elosEventToJsonObject(test->result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
