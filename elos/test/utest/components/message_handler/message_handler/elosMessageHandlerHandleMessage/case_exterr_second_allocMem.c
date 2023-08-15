// SPDX-License-Identifier: MIT
#include <cmocka_mocks/mock_libc.h>
#include <elos/eloslog/eloslog.h>
#include <elos/logger/logger.h>

#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageExterrSecondAllocMemSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    elosMessageHandlerHandleMessageUtestCreateMessage(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageExterrSecondAllocMemTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    elosMessageHandlerHandleMessageUtestDeleteMessage(state);
    return 0;
}

void elosTestElosMessageHandlerHandleMessageExterrSecondAllocMem(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientConnection_t *conn = testState->connection;
    safuResultE_t retval;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the second safuAllocMem fails");

    expect_value(__wrap_safuRecvExactly, fd, conn->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));
    will_set_parameter(__wrap_safuRecvExactly, buf, testState->message);
    will_return(__wrap_safuRecvExactly, sizeof(elosMessage_t));
    MOCK_FUNC_AFTER_CALL(safuRecvExactly, 0);

    expect_not_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosMessage_t) + testState->messagePayloadLen + 1);
    will_return(__wrap_safuAllocMem, NULL);
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 5);

    expect_any(__wrap_free, ptr);
    MOCK_FUNC_AFTER_CALL(free, 7);

    retval = elosMessageHandlerHandleMessage(conn);
    assert_int_equal(retval, SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(safuAllocMem);
}
