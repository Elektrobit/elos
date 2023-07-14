// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"

int elosTestElosEventFromJsonObjectSuccessNullEventSourceSetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectSuccessNullEventSourceTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectSuccessNullEventSource(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    const char *testData =
        "{\"date\":[25,100],\"severity\":5,\"hardwareid\":\"localhost\","
        "\"classification\":42,"
        "\"messageCode\":200,\"payload\":\"payload\"}";
    const elosEvent_t expected = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {0},
        .severity = ELOS_SEVERITY_DEBUG,
        .classification = 42,
        .hardwareid = "localhost",
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "convert json object to event when event source is zero");

    test->object = json_tokener_parse(testData);
    assert_non_null(test->object);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(test->result->date.tv_sec, expected.date.tv_sec);
    assert_int_equal(test->result->date.tv_nsec, expected.date.tv_nsec);
    assert_ptr_equal(test->result->source.appName, expected.source.appName);
    assert_ptr_equal(test->result->source.fileName, expected.source.fileName);
    assert_int_equal(test->result->source.pid, expected.source.pid);
    assert_int_equal(test->result->severity, expected.severity);
    assert_int_equal(test->result->classification, expected.classification);
    assert_string_equal(test->result->hardwareid, expected.hardwareid);
    assert_int_equal(test->result->messageCode, expected.messageCode);
    assert_string_equal(test->result->payload, expected.payload);
}
