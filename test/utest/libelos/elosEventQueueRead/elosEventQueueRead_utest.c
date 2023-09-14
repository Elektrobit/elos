// SPDX-License-Identifier: MIT
#include "elosEventQueueRead_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventQueueReadErrConversion), TEST_CASE(elosTestElosEventQueueReadErrExchange),
        TEST_CASE(elosTestElosEventQueueReadErrParam),      TEST_CASE(elosTestElosEventQueueReadErrRequest),
        TEST_CASE(elosTestElosEventQueueReadSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    *state = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(*state);
    memset(*state, 0, sizeof(elosUnitTestState_t));
    test = *state;

    test->session.fd = _MOCK_FD;
    test->session.connected = true;

    test->requestStr = "{\"eventQueueId\":42}";
    test->requestSize = sizeof(elosMessage_t) + strlen(test->requestStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENTQUEUE_READ, test->requestStr, &test->request);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->responseStrNormal = "{\"error\":null,\"eventArray\":[{\"messageCode\":2000},{\"messageCode\":3000}]}";
    test->responseStrEmpty = "{\"error\":null,\"eventArray\":[]}";
    test->responseStrNoArray = "{\"error\":null}";

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->request);
    free(*state);

    return 0;
}

void elosMockSendAndReceiveJsonMessage(elosUnitTestState_t *test, char const *responseStr, safuResultE_t result) {
    // We do not mock elosSendAndReceiveJsonMessage directly here, as we want to check the message that is sent
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_value(elosSendMessage, session, &test->session);
    expect_memory(elosSendMessage, message, test->request, test->requestSize);
    will_return(elosSendMessage, result);

    if (result == SAFU_RESULT_OK) {
        json_object *responseObj = NULL;

        // elosLogFindEvent frees the jsonObject it created, so we have to give it one to free
        responseObj = json_tokener_parse(responseStr);
        assert_non_null(responseObj);

        MOCK_FUNC_AFTER_CALL(elosReceiveJsonMessage, 0);
        expect_value(elosReceiveJsonMessage, session, &test->session);
        expect_value(elosReceiveJsonMessage, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
        expect_not_value(elosReceiveJsonMessage, jsonObject, NULL);
        will_set_parameter(elosReceiveJsonMessage, jsonObject, responseObj);
        will_return(elosReceiveJsonMessage, result);
    }
}