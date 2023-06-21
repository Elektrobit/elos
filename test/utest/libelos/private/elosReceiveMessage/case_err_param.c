// SPDX-License-Identifier: MIT

#include "elosReceiveMessage_utest.h"

int elosTestElosReceiveMessageErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosReceiveMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveMessageErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    safuResultE_t result;

    TEST("elosReceiveMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosReceiveMessage(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &message");
    result = elosReceiveMessage(NULL, &test->message);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, NULL");
    result = elosReceiveMessage(&invalidSession, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, &message");
    result = elosReceiveMessage(&invalidSession, &test->message);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&session, NULL");
    result = elosReceiveMessage(&test->session, NULL);
    assert_true(result == SAFU_RESULT_FAILED);
}
