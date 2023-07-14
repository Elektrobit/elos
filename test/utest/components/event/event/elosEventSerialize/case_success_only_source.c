// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeSuccessOnlySourceSetup(void **state) {
    *state = NULL;
    return 0;
}

int elosTestElosEventSerializeSuccessOnlySourceTeardown(void **state) {
    char *result = *(char **)state;
    free(result);
    return 0;
}

void elosTestElosEventSerializeSuccessOnlySource(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    char *result = NULL;
    elosEventSource_t testEventSource = {
        .appName = "appName",
        .fileName = "usr/bin",
        .pid = 234,
    };

    const char *expectedData = "{\"Source\":{\"appName\":\"appName\",\"fileName\":\"usr\\/bin\",\"pid\":234}}";
    const elosEvent_t testEvent = {.source = testEventSource};

    TEST("elosEventSerialize");
    SHOULD("%s", "Serialize the given event with only source to corresponding string");

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_non_null(result);
    assert_string_equal(result, expectedData);

    *state = result;
}
