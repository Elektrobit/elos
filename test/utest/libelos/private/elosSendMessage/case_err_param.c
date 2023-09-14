// SPDX-License-Identifier: MIT

#include "elosSendMessage_utest.h"

int elosTestElosSendMessageErrParamSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosSendMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendMessageErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    safuResultE_t result;

    TEST("elosSendMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosSendMessage(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, NULL");
    result = elosSendMessage(&invalidSession, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, &message");
    result = elosSendMessage(&invalidSession, test->message);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &message");
    result = elosSendMessage(NULL, test->message);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&session, NULL");
    result = elosSendMessage(&test->session, NULL);
    assert_true(result == SAFU_RESULT_FAILED);
}
