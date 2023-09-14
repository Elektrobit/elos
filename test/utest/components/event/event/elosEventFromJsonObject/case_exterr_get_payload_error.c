// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventFromJsonObjectPayloadErrorSetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectPayloadErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectPayloadError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED as getting payload fails");

    test->object = json_tokener_parse("{\"payload\":\"1234\"}");
    assert_non_null(test->object);

    MOCK_FUNC_AFTER_CALL(safuJsonGetString, 0);
    expect_any(__wrap_safuJsonGetString, jobj);
    expect_string(__wrap_safuJsonGetString, name, "payload");
    expect_any(__wrap_safuJsonGetString, idx);
    expect_any(__wrap_safuJsonGetString, val);
    will_set_parameter(__wrap_safuJsonGetString, val, "");
    will_return(__wrap_safuJsonGetString, 1);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
