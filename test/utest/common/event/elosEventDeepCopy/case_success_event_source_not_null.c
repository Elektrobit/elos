// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int elosTestElosEventSuccessEventSourceNotNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSuccessEventSourceNotNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSuccessEventSourceNotNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventSource_t testEventSource = {
        .appName = "appName",
        .fileName = "usr/bin",
        .pid = 234,
    };
    const elosEvent_t testSource = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = testEventSource,
        .severity = ELOS_SEVERITY_DEBUG,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "copy given source event with event source to target event");

    result = elosEventDeepCopy(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testDest.date.tv_sec, testSource.date.tv_sec);
    assert_int_equal(testDest.date.tv_nsec, testSource.date.tv_nsec);
    assert_int_equal(testDest.severity, testSource.severity);

    assert_string_equal(testDest.source.appName, testSource.source.appName);
    assert_string_equal(testDest.source.fileName, testSource.source.fileName);
    assert_int_equal(testDest.source.pid, testSource.source.pid);
    assert_string_equal(testDest.hardwareid, testSource.hardwareid);
    assert_int_equal(testDest.classification, testSource.classification);
    assert_int_equal(testDest.messageCode, testSource.messageCode);
    assert_string_equal(testDest.payload, testSource.payload);

    elosEventDeleteMembers(&testDest);
}
