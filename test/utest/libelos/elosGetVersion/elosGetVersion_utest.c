// SPDX-License-Identifier: MIT
#include "elosGetVersion_utest.h"

#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosGetVersionErrCall),
        TEST_CASE(elosTestElosGetVersionErrParam),
        TEST_CASE(elosTestElosGetVersionSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));
    test->session.connected = true;

    result = elosCreateMessage(ELOS_MESSAGE_GET_VERSION, NULL, &test->requestMsg);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->emptyResponseStr = "{}";
    test->normalResponseStr = "{\"error\":null, \"version\":\"" MOCK_VERSIONSTR "\"}";

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->requestMsg);
    free(*state);

    return 0;
}

void elosMockSendMessage(elosUnitTestState_t *test, safuResultE_t result) {
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_value(elosSendMessage, session, &test->session);
    expect_memory(elosSendMessage, message, test->requestMsg, sizeof(elosMessage_t));
    will_return(elosSendMessage, result);
}

void elosMockReceiveJsonMessage(elosUnitTestState_t *test, char const *responseStr, safuResultE_t result) {
    json_object *responseObj = NULL;

    if (responseStr != NULL) {
        // elosGetVersion frees the jsonObject it created, so we have to give it one where it is able to do so
        responseObj = json_tokener_parse(responseStr);
        assert_non_null(responseObj);
    }

    MOCK_FUNC_AFTER_CALL(elosReceiveJsonMessage, 0);
    expect_value(elosReceiveJsonMessage, session, &test->session);
    expect_value(elosReceiveJsonMessage, messageId, ELOS_MESSAGE_RESPONSE_GET_VERSION);
    expect_any(elosReceiveJsonMessage, jsonObject);
    will_set_parameter(elosReceiveJsonMessage, jsonObject, responseObj);
    will_return(elosReceiveJsonMessage, result);
}