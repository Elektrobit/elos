// SPDX-License-Identifier: MIT

#include "elosSendAndReceiveJsonMessage_utest.h"

int elosTestElosSendAndReceiveJsonMessageErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendAndReceiveJsonMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendAndReceiveJsonMessageErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    elosTestSet_t *tSet = &test->normal;
    safuResultE_t result;

    TEST("elosSendAndReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    // Note: The fourth parameter, jsonStr, is allowed to be NULL and doesn't need to be tested here

    PARAM("%s", "NULL, 0, NULL, NULL");
    result = elosSendAndReceiveJsonMessage(NULL, 0, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, messageId, NULL, NULL");
    result = elosSendAndReceiveJsonMessage(NULL, tSet->requestId, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, messageId, jsonObject, NULL");
    result = elosSendAndReceiveJsonMessage(NULL, tSet->requestId, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, 0, jsonObject, NULL");
    result = elosSendAndReceiveJsonMessage(NULL, 0, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidSession, messageId, NULL, NULL");
    result = elosSendAndReceiveJsonMessage(&invalidSession, tSet->requestId, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidSession, messageId, jsonObject, NULL");
    result = elosSendAndReceiveJsonMessage(&invalidSession, tSet->requestId, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidSession, 0, jsonObject, NULL");
    result = elosSendAndReceiveJsonMessage(&invalidSession, 0, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "session, messageId, NULL, NULL");
    result = elosSendAndReceiveJsonMessage(&test->session, tSet->requestId, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "session, 0, jsonObject, NULL");
    result = elosSendAndReceiveJsonMessage(&test->session, 0, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
