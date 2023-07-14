// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "elosEventSourceSerialize_utest.h"

int elosTestElosEventSourceSerializeSuccessUninitializedPIDSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceSerializeSuccessUninitializedPIDTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    free(test->receivedString);
    return 0;
}

void elosTestElosEventSourceSerializeSuccessUninitializedPID(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t expected = {.appName = "appNameTest", .fileName = "fileNameTest", .pid = 0};

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "succeed when only the event pid is NULL");

    result = elosEventSourceSerialize(&test->receivedString, &expected);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventSourceDeserialize(&test->source, test->receivedString);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(test->source.appName, expected.appName);
    assert_string_equal(test->source.fileName, expected.fileName);
    assert_int_equal(test->source.pid, expected.pid);
}
