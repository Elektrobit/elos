// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeSuccessOnlySeveritySetup(void **state) {
    *state = NULL;
    return 0;
}

int elosTestElosEventSerializeSuccessOnlySeverityTeardown(void **state) {
    char *result = *(char **)state;
    free(result);
    return 0;
}

void elosTestElosEventSerializeSuccessOnlySeverity(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    char *result = NULL;
    const char *expectedData = "{\"source\":{},\"severity\":5}";
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};

    TEST("elosEventSerialize");
    SHOULD("%s", "Serialize the given event with only severity to corresponding string");

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_non_null(result);
    assert_string_equal(result, expectedData);

    *state = result;
}
