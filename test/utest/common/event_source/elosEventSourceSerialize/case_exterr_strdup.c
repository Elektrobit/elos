// SPDX-License-Identifier: MIT

#include "elosEventSourceSerialize_utest.h"

int elosTestElosEventSourceSerializeExterrStrdupSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceSerializeExterrStrdupTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    free(test->receivedString);
    return 0;
}

void elosTestElosEventSourceSerializeExterrStrdup(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSource_t source = {.appName = "appNameTest", .fileName = "fileNameTest", .pid = 123};

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the allocation of the result string fails");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_any(__wrap_strdup, string);
    will_return(__wrap_strdup, NULL);

    result = elosEventSourceSerialize(&test->receivedString, &source);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
