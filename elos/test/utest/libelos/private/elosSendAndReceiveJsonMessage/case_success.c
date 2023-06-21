// SPDX-License-Identifier: MIT

#include "elosSendAndReceiveJsonMessage_utest.h"

int elosTestElosSendAndReceiveJsonMessageSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendAndReceiveJsonMessageSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendAndReceiveJsonMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t *tSet = &test->normal;
    char const *bufferStr;
    safuResultE_t result;

    TEST("elosSendAndReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour of elosSendAndReceiveJsonMessage");

    PARAM("%s", "send and receive");
    elosMockSendJsonMessage(test, tSet->requestId, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, tSet->responseId, tSet->responseObj, SAFU_RESULT_OK);
    result = elosSendAndReceiveJsonMessage(&test->session, tSet->requestId, tSet->requestObj, &test->bufferObj);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->bufferObj);
    bufferStr = json_object_to_json_string_ext(test->bufferObj, JSON_C_TO_STRING_PLAIN);
    assert_non_null(bufferStr);
    assert_string_equal(bufferStr, tSet->responseStr);

    PARAM("%s", "send, receive and discard");
    elosMockSendJsonMessage(test, tSet->requestId, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, tSet->responseId, NULL, SAFU_RESULT_OK);
    result = elosSendAndReceiveJsonMessage(&test->session, tSet->requestId, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_OK);
}
