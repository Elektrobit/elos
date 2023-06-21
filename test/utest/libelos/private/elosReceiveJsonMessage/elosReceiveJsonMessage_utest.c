// SPDX-License-Identifier: MIT
#include "elosReceiveJsonMessage_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosReceiveJsonMessageErrJson),
        TEST_CASE(elosTestElosReceiveJsonMessageErrParam),
        TEST_CASE(elosTestElosReceiveJsonMessageErrReceive),
        TEST_CASE(elosTestElosReceiveJsonMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    test->normal.jsonStr = "{\"payload\":\"test123\",\"error\":null}";
    test->normal.messageSize = sizeof(elosMessage_t) + strlen(test->normal.jsonStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, test->normal.jsonStr, &test->normal.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->empty.jsonStr = "{}";
    test->empty.messageSize = sizeof(elosMessage_t) + strlen(test->empty.jsonStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, test->empty.jsonStr, &test->empty.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->error.jsonStr = "{\"error\":\"unit test demonstration error\"}";
    test->error.messageSize = sizeof(elosMessage_t) + strlen(test->error.jsonStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, test->error.jsonStr, &test->error.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->nojson.messageSize = sizeof(elosMessage_t);
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, NULL, &test->nojson.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->session.connected = true;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->normal.message);
    free(test->empty.message);
    free(test->error.message);
    free(test->nojson.message);
    free(*state);

    return 0;
}

safuResultE_t elosMessageClone(elosMessage_t **message, elosMessage_t const *from) {
    size_t const messageSize = sizeof(elosMessage_t) + from->length;
    elosMessage_t *newMessage;
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (message != NULL) {
        newMessage = safuAllocMem(NULL, messageSize);
        if (newMessage != NULL) {
            memcpy(newMessage, from, messageSize);
            *message = newMessage;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

void elosMockReceive(elosUnitTestState_t *test, elosMessage_t *message, safuResultE_t result) {
    safuResultE_t resVal;

    // elosReceiveJsonMessage will free the message it created, so we clone the message here for the mocking
    // Also has to be done BEFORE the mocking macros, otherwise we'll get a couple memory access errors.
    if (message == NULL) {
        test->clonedMessage = NULL;
    } else {
        resVal = elosMessageClone(&test->clonedMessage, message);
        assert_int_equal(resVal, SAFU_RESULT_OK);
    }

    MOCK_FUNC_AFTER_CALL(elosReceiveMessage, 0);
    expect_value(elosReceiveMessage, session, &test->session);
    expect_not_value(elosReceiveMessage, message, NULL);
    will_set_parameter(elosReceiveMessage, message, test->clonedMessage);
    will_return(elosReceiveMessage, result);
}
