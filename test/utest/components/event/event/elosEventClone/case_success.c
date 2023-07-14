// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int elosTestElosEventCloneSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventCloneSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventCloneSuccess(UNUSED void **state) {
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
    elosEvent_t *testDest = NULL;

    TEST("elosEventClone");
    SHOULD("%s", "copy source event to target");

    result = elosEventClone(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testDest->date.tv_sec, testSource.date.tv_sec);
    assert_int_equal(testDest->date.tv_nsec, testSource.date.tv_nsec);
    assert_null(testDest->source.appName);
    assert_null(testDest->source.fileName);
    assert_int_equal(testDest->source.pid, 0);
    assert_int_equal(testDest->severity, testSource.severity);
    assert_string_equal(testDest->hardwareid, testSource.hardwareid);
    assert_int_equal(testDest->classification, testSource.classification);
    assert_int_equal(testDest->messageCode, testSource.messageCode);
    assert_string_equal(testDest->payload, testSource.payload);

    elosEventDelete(testDest);
}
