// SPDX-License-Identifier: MIT
#include "elosSendAndReceiveJsonMessage_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosSendAndReceiveJsonMessageErrCall),
        TEST_CASE(elosTestElosSendAndReceiveJsonMessageErrParam),
        TEST_CASE(elosTestElosSendAndReceiveJsonMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    elosTestSet_t *tSet = NULL;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    tSet = &test->normal;
    tSet->requestId = ELOS_MESSAGE_EVENT_PUBLISH;
    tSet->requestStr = "{\"payload\":\"test123\"}";
    tSet->requestObj = json_tokener_parse(tSet->requestStr);
    assert_non_null(tSet->requestObj);
    tSet->responseId = ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH;
    tSet->responseStr = "{\"error\":null}";
    tSet->responseObj = json_tokener_parse(tSet->responseStr);
    assert_non_null(tSet->responseObj);

    test->session.connected = true;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    json_object_put(test->normal.requestObj);
    json_object_put(test->normal.responseObj);
    free(*state);

    return 0;
}

void elosMockSendJsonMessage(elosUnitTestState_t *test, uint8_t id, safuResultE_t result) {
    MOCK_FUNC_AFTER_CALL(elosSendJsonMessage, 0);
    expect_value(elosSendJsonMessage, session, &test->session);
    expect_value(elosSendJsonMessage, messageId, id);
    expect_not_value(elosSendJsonMessage, jsonObject, NULL);
    will_return(elosSendJsonMessage, result);
}

void elosMockReceiveJsonMessage(elosUnitTestState_t *test, uint8_t id, json_object *jObj, safuResultE_t result) {
    MOCK_FUNC_AFTER_CALL(elosReceiveJsonMessage, 0);
    expect_value(elosReceiveJsonMessage, session, &test->session);
    expect_value(elosReceiveJsonMessage, messageId, id);
    expect_any(elosReceiveJsonMessage, jsonObject);
    if (jObj != NULL) {
        will_set_parameter(elosReceiveJsonMessage, jsonObject, jObj);
    }
    will_return(elosReceiveJsonMessage, result);
}
