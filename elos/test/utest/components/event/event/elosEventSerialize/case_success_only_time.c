// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeSuccessOnlyTimeSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeSuccessOnlyTimeTeardown(UNUSED void **state) {
    return 0;
}

static void _runElosEventSerializeSuccessOnlyTime(const elosEvent_t *testEvent) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    char *result = NULL;
    const char *expectedData;

    if (testEvent->date.tv_sec == 0) {
        expectedData = "{\"date\":[0,100],\"Source\":{}}";
    } else {
        expectedData = "{\"date\":[39,0],\"Source\":{}}";
    }

    retval = elosEventSerialize(&result, testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_non_null(result);
    assert_string_equal(result, expectedData);

    free(result);
}

void elosTestElosEventSerializeSuccessOnlyTime(UNUSED void **state) {
    const elosEvent_t testEvent[] = {
        {.date = {.tv_sec = 0, .tv_nsec = 100}},
        {.date = {.tv_sec = 39, .tv_nsec = 0}},
    };

    TEST("elosEventSerialize");
    SHOULD("%s", "Serialize the given event with date value zero to corresponding string");

    _runElosEventSerializeSuccessOnlyTime(&testEvent[0]);
    _runElosEventSerializeSuccessOnlyTime(&testEvent[1]);
}
