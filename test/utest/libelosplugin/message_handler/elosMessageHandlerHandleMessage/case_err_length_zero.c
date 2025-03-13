// SPDX-License-Identifier: MIT

#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageErrLengthZeroSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosUteststateT_t *testState = *state;
    const char *payload = "{\"key\":\"value\"}";
    size_t payloadSize = strlen(payload);
    size_t messageLength = sizeof(elosMessage_t) + payloadSize;

    elosMessage_t *newMessage = safuAllocMem(NULL, messageLength);
    assert_int_not_equal(newMessage, NULL);

    memset(newMessage, 0, messageLength);
    newMessage->version = ELOS_PROTOCOL_VERSION;
    newMessage->message = ELOS_MESSAGE_EVENT_PUBLISH;
    newMessage->length = 0;
    memcpy(newMessage->json, payload, payloadSize);

    testState->message = newMessage;
    testState->messagePayloadLen = payloadSize;

    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrLengthZeroTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

static void _testElosMessageHandlerHandleMessageErrLengthZeroParam(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->message;
    safuResultE_t retval;

    PARAM("message id = %x", msg->message);

    MOCK_FUNC_ALWAYS(safuRecvExactly);
    expect_value(__wrap_safuRecvExactly, fd, testState->connection->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));
    will_set_parameter(__wrap_safuRecvExactly, buf, testState->message);
    will_set_parameter(__wrap_safuRecvExactly, transferred, sizeof(elosMessage_t));
    will_return(__wrap_safuRecvExactly, SAFU_RESULT_OK);

    retval = elosMessageHandlerHandleMessage(conn);

    MOCK_FUNC_NEVER(safuRecvExactly);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}

void elosTestElosMessageHandlerHandleMessageErrLengthZero(void **state) {
    elosUteststateT_t *testState = *state;
    elosMessage_t *msg = testState->message;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "fail the call to the message handler with length 0");

    msg->message = ELOS_MESSAGE_EVENT_PUBLISH;
    _testElosMessageHandlerHandleMessageErrLengthZeroParam(state);

    msg->message = ELOS_MESSAGE_EVENT_SUBSCRIBE;
    _testElosMessageHandlerHandleMessageErrLengthZeroParam(state);

    msg->message = ELOS_MESSAGE_LOG_FIND_EVENT;
    _testElosMessageHandlerHandleMessageErrLengthZeroParam(state);

    msg->message = ELOS_MESSAGE_EVENTQUEUE_READ;
    _testElosMessageHandlerHandleMessageErrLengthZeroParam(state);

    msg->message = ELOS_MESSAGE_EVENT_UNSUBSCRIBE;
    _testElosMessageHandlerHandleMessageErrLengthZeroParam(state);
}
