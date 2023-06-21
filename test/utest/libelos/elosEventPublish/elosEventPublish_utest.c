// SPDX-License-Identifier: MIT
#include "elosEventPublish_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventPublishErrNewObject),     TEST_CASE(elosTestElosEventPublishErrConvert),
        TEST_CASE(elosTestElosEventPublishErrCommunication), TEST_CASE(elosTestElosEventPublishErrParam),
        TEST_CASE(elosTestElosEventPublishSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    elosUnitTestSet_t *tSet;
    safuResultE_t result;
    elosEvent_t event = {
        .messageCode = 1,
        .severity = 2,
        .payload = "123",
    };

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    tSet = &test->normal;
    tSet->event = event;
    result = elosEventSerialize(&tSet->eventStr, &tSet->event);
    assert_int_equal(result, SAFU_RESULT_OK);
    tSet->messageSize = sizeof(elosMessage_t) + strlen(tSet->eventStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, tSet->eventStr, &tSet->message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->session.connected = true;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->normal.eventStr);
    free(test->normal.message);
    free(*state);

    return 0;
}

void elosMockSendAndReceiveJsonMessage(elosUnitTestState_t *test, elosUnitTestSet_t *tSet, safuResultE_t result) {
    // We do not mock elosSendAndReceiveJsonMessage directly here, as we want to check the message that is sent
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_value(elosSendMessage, session, &test->session);
    expect_memory(elosSendMessage, message, tSet->message, tSet->messageSize);
    will_return(elosSendMessage, result);

    if (result == SAFU_RESULT_OK) {
        MOCK_FUNC_AFTER_CALL(elosReceiveJsonMessage, 0);
        expect_value(elosReceiveJsonMessage, session, &test->session);
        expect_value(elosReceiveJsonMessage, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
        expect_value(elosReceiveJsonMessage, jsonObject, NULL);
        will_return(elosReceiveJsonMessage, result);
    }
}
