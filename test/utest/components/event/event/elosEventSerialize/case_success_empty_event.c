// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeSuccessEmptyEventSetup(void **state) {
    *state = NULL;
    return 0;
}

int elosTestElosEventSerializeSuccessEmptyEventTeardown(void **state) {
    char *result = *(char **)state;
    free(result);
    return 0;
}

void elosTestElosEventSerializeSuccessEmptyEvent(void **state) {
    char *result = NULL;
    const elosEvent_t testEvent = {0};
    safuResultE_t retval = SAFU_RESULT_FAILED;

    TEST("elosEventSerialize");
    SHOULD("%s", "Serialize the given null event to corresponding string");

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_non_null(result);
    assert_string_equal(result, "{\"source\":{}}");

    *state = result;
}
