// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeSuccessNullEventSourceSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeSuccessNullEventSourceTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeSuccessNullEventSource(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};
    const char *testData =
        "{\"date\":[25,100],\"severity\":5,\"hardwareid\":\"localhost\","
        "\"classification\":42,\"messageCode\":200,\"payload\":\"payload\"}";

    const elosEvent_t expectedEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {0},
        .severity = ELOS_SEVERITY_DEBUG,
        .classification = 42,
        .hardwareid = "localhost",
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };

    TEST("elosEventDeserialize");
    SHOULD("%s", "deserialize json string to event when event source is null");

    retval = elosEventDeserialize(&resultEvent, testData);

    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(resultEvent.date.tv_sec, expectedEvent.date.tv_sec);
    assert_int_equal(resultEvent.date.tv_nsec, expectedEvent.date.tv_nsec);
    assert_null(resultEvent.source.appName);
    assert_null(resultEvent.source.fileName);
    assert_int_equal(resultEvent.source.pid, 0);
    assert_int_equal(resultEvent.severity, expectedEvent.severity);
    assert_int_equal(resultEvent.classification, expectedEvent.classification);
    assert_string_equal(resultEvent.hardwareid, expectedEvent.hardwareid);
    assert_int_equal(resultEvent.messageCode, expectedEvent.messageCode);
    assert_string_equal(resultEvent.payload, expectedEvent.payload);

    elosEventDeleteMembers(&resultEvent);
}
