// SPDX-License-Identifier: MIT
#include "elosEventSubscribe_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventSubscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventSubscribeErrEmptyList),
        TEST_CASE(elosTestElosEventSubscribeErrInvalidParameter),
        TEST_CASE(elosTestElosEventSubscribeErrorResponseHasNoIdsField),
        TEST_CASE(elosTestElosEventSubscribeExterrCommunicationFailed),
        TEST_CASE(elosTestElosEventSubscribeExterrJsonObjectNewObjectFailed),
        TEST_CASE(elosTestElosEventSubscribeExterrJsonAddNewArrayFailed),
        TEST_CASE(elosTestElosEventSubscribeExterrJsonAddNewStringFailed),
        TEST_CASE(elosTestElosEventSubscribeExterrJsonGetUint32Failed),
        // TEST_CASE(elosTestElosEventSubscribeSuccess),
    };

    return RUN_TEST_SUITE(tests, elosEventSubscribeUtest);
}

static int elosEventSubscribeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosEventSubscribeUtestTeardown(UNUSED void **state) {
    return 0;
}

void elosEventSubscribeUtestCreateMessage(elosMessage_t **message, size_t *messageLength, uint8_t messageId,
                                          const char *payload, size_t payloadSize) {
    elosMessage_t *newMessage = NULL;
    *messageLength = sizeof(elosMessage_t) + payloadSize + 1;

    newMessage = safuAllocMem(NULL, *messageLength);
    assert_non_null_msg(newMessage, "failed to allocate elos_message");

    memset(newMessage, 0, *messageLength);
    newMessage->version = ELOS_PROTOCOL_VERSION;
    newMessage->message = messageId;
    newMessage->length = payloadSize + 1;
    memcpy(newMessage->json, payload, payloadSize + 1);

    *message = newMessage;
}

void elosMockSendAndReceiveJsonMessage(_TestState_t *test, safuResultE_t result) {
    // We do not mock elosSendAndReceiveJsonMessage directly here, as we want to check the message that is sent
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_value(elosSendMessage, session, &test->session);
    expect_memory(elosSendMessage, message, test->request.message, test->request.length);
    will_return(elosSendMessage, result);

    if (result == SAFU_RESULT_OK) {
        MOCK_FUNC_AFTER_CALL(elosReceiveMessage, 0);
        expect_value(elosReceiveMessage, session, &test->session);
        expect_not_value(elosReceiveMessage, message, NULL);
        will_set_parameter(elosReceiveMessage, message, test->response.message);
        will_return(elosReceiveMessage, result);
    }
}
