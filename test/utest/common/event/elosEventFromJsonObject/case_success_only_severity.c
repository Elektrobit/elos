// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"

int elosTestElosEventFromJsonObjectSuccessSeverityOnlySetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectSuccessSeverityOnlyTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectSuccessSeverityOnly(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    const char *testData = "{\"severity\":5}";
    const elosEvent_t expected = {.severity = ELOS_SEVERITY_DEBUG};

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "convert to event when json object has only severity");

    test->object = json_tokener_parse(testData);
    assert_non_null(test->object);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(test->result->date.tv_sec, expected.date.tv_sec);
    assert_int_equal(test->result->date.tv_nsec, expected.date.tv_nsec);
    assert_int_equal(test->result->severity, expected.severity);
    assert_int_equal(test->result->classification, expected.classification);
    assert_ptr_equal(test->result->hardwareid, expected.hardwareid);
    assert_int_equal(test->result->messageCode, expected.messageCode);
    assert_ptr_equal(test->result->payload, expected.payload);
}
