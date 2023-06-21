// SPDX-License-Identifier: MIT

#include "elosReceiveJsonMessage_utest.h"

int elosTestElosReceiveJsonMessageErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosReceiveJsonMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveJsonMessageErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    uint8_t messageId = ELOS_MESSAGE_GET_VERSION;
    safuResultE_t result;

    TEST("elosReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    // The third parameter, jsonObject, can be NULL and is thus not tested here

    PARAM("%s", "NULL, 0, NULL");
    result = elosReceiveJsonMessage(NULL, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, 0, NULL");
    result = elosReceiveJsonMessage(&invalidSession, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, messageId, NULL");
    result = elosReceiveJsonMessage(&invalidSession, messageId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, messageId, NULL");
    result = elosReceiveJsonMessage(NULL, messageId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, 0, NULL");
    result = elosReceiveJsonMessage(&test->session, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
