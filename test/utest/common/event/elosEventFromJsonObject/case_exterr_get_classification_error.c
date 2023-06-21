// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventFromJsonObjectClassificationErrorSetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectClassificationErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectClassificationError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED as getting classification fails");

    MOCK_FUNC_AFTER_CALL(safuJsonGetUint64, 0);
    expect_any(__wrap_safuJsonGetUint64, jobj);
    expect_string(__wrap_safuJsonGetUint64, name, "classification");
    expect_any(__wrap_safuJsonGetUint64, idx);
    expect_any(__wrap_safuJsonGetUint64, val);
    will_set_parameter(__wrap_safuJsonGetUint64, val, 0);
    will_return(__wrap_safuJsonGetUint64, -1);

    test->object = json_tokener_parse("{\"classification\":\"INVALIDTYPE\"}");
    assert_non_null(test->object);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
