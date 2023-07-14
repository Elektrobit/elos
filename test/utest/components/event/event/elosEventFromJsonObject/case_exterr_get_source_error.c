// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"
#include "mock_event_source.h"

int elosTestElosEventFromJsonObjectSourceErrorSetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectSourceErrorTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectSourceError(void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED as retrieving event source fails");

    test->object = json_tokener_parse("{\"Source\":\"INVALIDVALUE\"}");
    assert_non_null(test->object);

    MOCK_FUNC_AFTER_CALL(elosEventSourceFromJsonObject, 0);
    expect_not_value(elosEventSourceFromJsonObject, to, NULL);
    expect_not_value(elosEventSourceFromJsonObject, from, NULL);
    will_return(elosEventSourceFromJsonObject, SAFU_RESULT_FAILED);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
