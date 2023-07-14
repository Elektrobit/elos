// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeSuccessUninitializedFileNameSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeSuccessUninitializedFileNameTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeSuccessUninitializedFileName(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char string[] = "{\"appName\":\"mockAppName\",\"pid\":123}";
    const elosEventSource_t expected = {.appName = "mockAppName", .fileName = NULL, .pid = 123};
    elosEventSource_t receivedSource = {0};

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "succeed when only the jsonString fileName is missing");

    result = elosEventSourceDeserialize(&receivedSource, string);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(receivedSource.appName, expected.appName);
    assert_ptr_equal(receivedSource.fileName, expected.fileName);
    assert_int_equal(receivedSource.pid, expected.pid);

    free(receivedSource.appName);
}
