// SPDX-License-Identifier: MIT

#include "elosCreateMessage_utest.h"

int elosTestElosCreateMessageSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    test->message = NULL;

    return 0;
}

int elosTestElosCreateMessageSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    test->message = NULL;

    return 0;
}

void elosTestElosCreateMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    char const *jsonStr = "{}";
    size_t const length = strlen(jsonStr) + 1;

    TEST("elosCreateMessage");
    SHOULD("%s", "test correct behaviour of elosCreateMessage");

    PARAM("%s", "create message with jsonStr");
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, jsonStr, &test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->message->version, ELOS_PROTOCOL_VERSION);
    assert_int_equal(test->message->message, ELOS_MESSAGE_EVENT_PUBLISH);
    assert_int_equal(test->message->length, length);
    assert_string_equal(test->message->json, jsonStr);

    free(test->message);
    test->message = NULL;

    PARAM("%s", "create message without jsonStr");
    result = elosCreateMessage(ELOS_MESSAGE_GET_VERSION, NULL, &test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null_msg(test->message, "message was unexpectedly still null");
    assert_int_equal(test->message->version, ELOS_PROTOCOL_VERSION);
    assert_int_equal(test->message->message, ELOS_MESSAGE_GET_VERSION);
    assert_int_equal(test->message->length, 0);
}
