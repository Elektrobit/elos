// SPDX-License-Identifier: MIT
#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageErrReadPayloadFailedSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosMessageHandlerHandleMessageUtestCreateMessage(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrReadPayloadFailedTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

void elosTestElosMessageHandlerHandleMessageErrReadPayloadFailed(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientConnection_t *conn = testState->connection;
    safuResultE_t retval;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosMessageHandlerReadPayload fails");

    MOCK_FUNC_ALWAYS(safuRecvExactly);
    expect_value(__wrap_safuRecvExactly, fd, conn->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));
    will_set_parameter(__wrap_safuRecvExactly, buf, testState->message);
    will_return(__wrap_safuRecvExactly, sizeof(elosMessage_t));

    expect_value(__wrap_safuRecvExactly, fd, conn->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, testState->messagePayloadLen);
    will_set_parameter(__wrap_safuRecvExactly, buf, testState->message->json);
    will_return(__wrap_safuRecvExactly, -1);

    retval = elosMessageHandlerHandleMessage(conn);
    assert_int_equal(retval, SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(safuRecvExactly);
}
