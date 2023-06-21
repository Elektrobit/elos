// SPDX-License-Identifier: MIT

#include "elosMessageHandlerHandleMessage_utest.h"
#include "mock_message_handler.h"

int elosTestElosMessageHandlerHandleMessageErrMessageFunctionReturnFailureSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosMessageHandlerHandleMessageUtestCreateMessage(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrMessageFunctionReturnFailureTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

void elosTestElosMessageHandlerHandleMessageErrMessageFunctionReturnFailure(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t retval;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the called message function fails");

    _RECV_MOCK(testState)

    PARAM("%s", "function = elosMessageEventPublish");
    expect_value(elosMessageEventPublish, conn, testState->connection);
    expect_not_value(elosMessageEventPublish, msg, NULL);
    will_return(elosMessageEventPublish, -1);
    MOCK_FUNC_AFTER_CALL(elosMessageEventPublish, 0);

    retval = elosMessageHandlerHandleMessage(testState->connection);

    assert_int_equal(retval, SAFU_RESULT_FAILED);
    _RECV_MOCK_CLEANUP
}
