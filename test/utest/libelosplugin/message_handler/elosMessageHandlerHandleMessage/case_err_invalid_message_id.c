// SPDX-License-Identifier: MIT

#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageErrInvalidMessageIdSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosMessageHandlerHandleMessageUtestCreateMessage(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrInvalidMessageIdTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

static void _testElosMessageHandlerHandleMessageErrInvalidMessageIdParam(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t retval;

    PARAM("message id = %x", testState->message->message);

    _RECV_MOCK(testState)
    retval = elosMessageHandlerHandleMessage(testState->connection);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
    _RECV_MOCK_CLEANUP
}

void elosTestElosMessageHandlerHandleMessageErrInvalidMessageId(void **state) {
    elosUteststateT_t *testState = *state;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED for an invalid message id");

    testState->message->message = 0;
    _testElosMessageHandlerHandleMessageErrInvalidMessageIdParam(state);
    testState->message->message = UINT8_MAX;
    _testElosMessageHandlerHandleMessageErrInvalidMessageIdParam(state);
}
