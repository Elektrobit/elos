// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeSuccessUninitializedPIDSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeSuccessUninitializedPIDTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeSuccessUninitializedPID(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char string[] = "{\n  \"appName\":\"mockAppName\",\n  \"fileName\":\"mockFileName\",\n}";
    const elosEventSource_t expected = {.appName = "mockAppName", .fileName = "mockFileName", .pid = 0};
    elosEventSource_t receivedSource = {0};

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "succeed when only the jsonString pid is missing");

    result = elosEventSourceDeserialize(&receivedSource, string);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(receivedSource.appName, expected.appName);
    assert_string_equal(receivedSource.fileName, expected.fileName);
    assert_int_equal(receivedSource.pid, expected.pid);

    free(receivedSource.appName);
    free(receivedSource.fileName);
}
