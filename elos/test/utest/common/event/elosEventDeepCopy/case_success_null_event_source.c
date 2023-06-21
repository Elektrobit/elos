// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int elosTestElosEventSuccessNullEventSourceSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSuccessNullEventSourceTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSuccessNullEventSource(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const elosEvent_t testSource = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {0},
        .severity = ELOS_SEVERITY_DEBUG,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "copy given source event with no event source to target event");

    result = elosEventDeepCopy(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testDest.date.tv_sec, testSource.date.tv_sec);
    assert_int_equal(testDest.date.tv_nsec, testSource.date.tv_nsec);
    assert_int_equal(testDest.severity, testSource.severity);
    assert_null(testDest.source.appName);
    assert_null(testDest.source.fileName);
    assert_int_equal(testDest.source.pid, 0);
    assert_string_equal(testDest.hardwareid, testSource.hardwareid);
    assert_int_equal(testDest.classification, testSource.classification);
    assert_int_equal(testDest.messageCode, testSource.messageCode);
    assert_string_equal(testDest.payload, testSource.payload);

    elosEventDeleteMembers(&testDest);
}
