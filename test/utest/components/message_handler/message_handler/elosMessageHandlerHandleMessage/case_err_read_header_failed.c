// SPDX-License-Identifier: MIT
#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageErrReadHeaderFailedSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrReadHeaderFailedTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    return 0;
}

void elosTestElosMessageHandlerHandleMessageErrReadHeaderFailed(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientConnection_t *conn = testState->connection;
    safuResultE_t retval;

    const char mockReceivedHeader[] = "1234";

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosMessageHandlerReadHeader fails");

    expect_value(__wrap_safuRecvExactly, fd, conn->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));
    will_set_parameter(__wrap_safuRecvExactly, buf, mockReceivedHeader);
    will_set_parameter(__wrap_safuRecvExactly, transferred, 0);
    will_return(__wrap_safuRecvExactly, SAFU_RESULT_FAILED);
    MOCK_FUNC_AFTER_CALL(safuRecvExactly, 0);

    retval = elosMessageHandlerHandleMessage(conn);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
