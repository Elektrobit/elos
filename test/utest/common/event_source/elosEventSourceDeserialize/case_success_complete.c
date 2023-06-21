// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeSuccessCompleteSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeSuccessCompleteTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeSuccessComplete(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char string[] = "{\"appName\":\"mockAppName\",\"fileName\":\"mockFileName\",\"pid\":123}";
    const elosEventSource_t expected = {.appName = "mockAppName", .fileName = "mockFileName", .pid = 123};
    elosEventSource_t receivedSource = {0};

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "succeed with all members contained in jsonString");

    result = elosEventSourceDeserialize(&receivedSource, string);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(receivedSource.appName, expected.appName);
    assert_string_equal(receivedSource.fileName, expected.fileName);
    assert_int_equal(receivedSource.pid, expected.pid);

    free(receivedSource.appName);
    free(receivedSource.fileName);
}
