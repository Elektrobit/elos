// SPDX-License-Identifier: MIT
#include "elosMessageHandlerHandleMessage_utest.h"

#define MOCK_FD 42

TEST_SUITE_FUNC_PROTOTYPES(elosMessageHandlerHandleMessageUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageHandlerHandleMessageErrConnection),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageExterrFirstAllocMem),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageErrReadHeaderFailed),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageExterrSecondAllocMem),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageErrReadPayloadFailed),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageErrInvalidMessageId),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageErrMessageFunctionReturnFailure),
        TEST_CASE(elosTestElosMessageHandlerHandleMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, elosMessageHandlerHandleMessageUtest);
}

void elosMessageHandlerHandleMessageUtestCreateConnection(void **state) {
    elosUteststateT_t *testState = *state;

    testState->connection = calloc(1, sizeof(elosClientConnection_t));
    int retval = testState->connection == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    testState->connection->fd = MOCK_FD;
}

void elosMessageHandlerHandleMessageUtestFreeConnection(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->connection);
}

void elosMessageHandlerHandleMessageUtestCreateMessage(void **state) {
    elosUteststateT_t *testState = *state;
    elosMessage_t *newMessage = NULL;
    uint8_t messageId = ELOS_MESSAGE_EVENT_PUBLISH;
    const char *payload = "{\"key\":\"value\"}";
    size_t payloadSize = strlen(payload);
    size_t messageLength = sizeof(elosMessage_t) + payloadSize;

    newMessage = safuAllocMem(NULL, messageLength);
    int retval = newMessage == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    memset(newMessage, 0, messageLength);
    newMessage->version = ELOS_PROTOCOL_VERSION;
    newMessage->message = messageId;
    newMessage->length = payloadSize;
    memcpy(newMessage->json, payload, payloadSize);

    testState->message = newMessage;
    testState->messagePayloadLen = payloadSize;
}

void elosMessageHandlerHandleMessageUtestDeleteMessage(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->message);
}

static int elosMessageHandlerHandleMessageUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    int retval = testState == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    *state = testState;
    return 0;
}

static int elosMessageHandlerHandleMessageUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
