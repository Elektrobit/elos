// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventFromJsonObjectDateErrorSetup(UNUSED void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectDateErrorTeardown(UNUSED void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectDateError(UNUSED void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED as getting date fails");

    test->object = json_tokener_parse("{\"date\":[25,100]}");
    assert_non_null(test->object);

    MOCK_FUNC_AFTER_CALL(safuJsonGetTimestamp, 0);
    expect_any(__wrap_safuJsonGetTimestamp, jobj);
    expect_string(__wrap_safuJsonGetTimestamp, name, "date");
    expect_any(__wrap_safuJsonGetTimestamp, idx);
    expect_any(__wrap_safuJsonGetTimestamp, timestamp);
    will_set_parameter(__wrap_safuJsonGetTimestamp, idx, 0);
    will_set_parameter(__wrap_safuJsonGetTimestamp, timestampm, 0);
    will_return(__wrap_safuJsonGetTimestamp, 1);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
