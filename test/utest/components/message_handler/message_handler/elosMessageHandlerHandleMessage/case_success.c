// SPDX-License-Identifier: MIT

#include "elosMessageHandlerHandleMessage_utest.h"
#include "mock_message_handler.h"

int elosTestElosMessageHandlerHandleMessageSuccessSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosMessageHandlerHandleMessageUtestCreateMessage(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageSuccessTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

typedef void elosSetupExpectedFunctionCall_t(elosClientManagerConnection_t const *const conn);

static void _expectMessageGetVersionCalled(elosClientManagerConnection_t const *const conn) {
    expect_value(elosMessageGetVersion, conn, conn);
    expect_not_value(elosMessageGetVersion, msg, NULL);
    will_return(elosMessageGetVersion, 0);
    MOCK_FUNC_AFTER_CALL(elosMessageGetVersion, 0);
}

static void _expectMessageEventPublish(elosClientManagerConnection_t const *const conn) {
    expect_value(elosMessageEventPublish, conn, conn);
    expect_not_value(elosMessageEventPublish, msg, NULL);
    will_return(elosMessageEventPublish, 0);
    MOCK_FUNC_AFTER_CALL(elosMessageEventPublish, 0);
}

static void _expectMessageLogFindEvent(elosClientManagerConnection_t const *const conn) {
    expect_value(elosMessageLogFindEvent, conn, conn);
    expect_not_value(elosMessageLogFindEvent, msg, NULL);
    will_return(elosMessageLogFindEvent, 0);
    MOCK_FUNC_AFTER_CALL(elosMessageLogFindEvent, 0);
}

static void _testElosMessageHandlerHandleMessageSuccessParam(
    void **state, elosSetupExpectedFunctionCall_t setupExpectedFunctionCall) {
    elosUteststateT_t *testState = *state;
    elosClientManagerConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->message;
    safuResultE_t retval;

    PARAM("message id = %x", msg->message);

    _RECV_MOCK(testState)

    setupExpectedFunctionCall(conn);

    retval = elosMessageHandlerHandleMessage(conn);

    assert_int_equal(retval, SAFU_RESULT_OK);

    _RECV_MOCK_CLEANUP
}

void elosTestElosMessageHandlerHandleMessageSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    elosMessage_t *msg = testState->message;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "successfully call the message handler with different values");

    msg->message = ELOS_MESSAGE_GET_VERSION;
    _testElosMessageHandlerHandleMessageSuccessParam(state, _expectMessageGetVersionCalled);

    msg->message = ELOS_MESSAGE_EVENT_PUBLISH;
    _testElosMessageHandlerHandleMessageSuccessParam(state, _expectMessageEventPublish);

    msg->message = ELOS_MESSAGE_LOG_FIND_EVENT;
    _testElosMessageHandlerHandleMessageSuccessParam(state, _expectMessageLogFindEvent);
}
