// SPDX-License-Identifier: MIT

#include "elosSendJsonMessage_utest.h"

int elosTestElosSendJsonMessageErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendJsonMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendJsonMessageErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    uint8_t messageId = ELOS_MESSAGE_GET_VERSION;
    safuResultE_t result;

    TEST("elosSendJsonMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, 0, NULL");
    result = elosSendJsonMessage(NULL, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, 0, NULL");
    result = elosSendJsonMessage(&invalidSession, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, messageId, NULL");
    result = elosSendJsonMessage(&invalidSession, messageId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, 0, &jsonObject");
    result = elosSendJsonMessage(&invalidSession, 0, test->normal.jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, 0, &jsonObject");
    result = elosSendJsonMessage(NULL, 0, test->normal.jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, messageId, &jsonObject");
    result = elosSendJsonMessage(NULL, messageId, test->normal.jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, 0, NULL");
    result = elosSendJsonMessage(&test->session, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, messageId, NULL");
    result = elosSendJsonMessage(&test->session, messageId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, 0, &jsonObject");
    result = elosSendJsonMessage(&test->session, 0, test->normal.jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
