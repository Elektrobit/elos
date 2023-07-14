// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "elosEventSourceSerialize_utest.h"

int elosTestElosEventSourceSerializeSuccessEmptySetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceSerializeSuccessEmptyTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    free(test->receivedString);
    return 0;
}

void elosTestElosEventSourceSerializeSuccessEmpty(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t expected = {.appName = NULL, .fileName = NULL, .pid = 0};

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "succeed for an empty elosEventSource_t");

    result = elosEventSourceSerialize(&test->receivedString, &expected);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventSourceDeserialize(&test->source, test->receivedString);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->source.appName, expected.appName);
    assert_int_equal(test->source.fileName, expected.fileName);
    assert_int_equal(test->source.pid, expected.pid);
}
