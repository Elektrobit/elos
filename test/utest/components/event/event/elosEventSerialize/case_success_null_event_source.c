// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeSuccessNullEventSourceSetup(void **state) {
    *state = NULL;
    return 0;
}

int elosTestElosEventSerializeSuccessNullEventSourceTeardown(void **state) {
    char *result = *(char **)state;
    free(result);
    return 0;
}

void elosTestElosEventSerializeSuccessNullEventSource(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    char *result = NULL;
    const char *expectedData =
        "{\"date\":[25,100],\"source\":{},\"severity\":5,"
        "\"hardwareid\":\"localhost\",\"classification\":42,\"messageCode\":200,\"payload\":\"payload\"}";

    const elosEvent_t testEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {0},
        .severity = ELOS_SEVERITY_DEBUG,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };

    TEST("elosEventSerialize");
    SHOULD("%s", "Serialize the given event with no event source to corresponding string");

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_non_null(result);
    assert_string_equal(result, expectedData);

    *state = result;
}
