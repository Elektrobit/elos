// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeSuccessUninitializedAppNameSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeSuccessUninitializedAppNameTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeSuccessUninitializedAppName(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char string[] = "{\"fileName\":\"mockFileName\",\"pid\":123}";
    const elosEventSource_t expected = {.appName = NULL, .fileName = "mockFileName", .pid = 123};
    elosEventSource_t receivedSource = {0};

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "succeed when only the jsonString appName is missing");

    result = elosEventSourceDeserialize(&receivedSource, string);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(receivedSource.appName, expected.appName);
    assert_string_equal(receivedSource.fileName, expected.fileName);
    assert_int_equal(receivedSource.pid, expected.pid);

    free(receivedSource.fileName);
}
