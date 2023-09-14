// SPDX-License-Identifier: MIT
#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageExterrFirstAllocMemSetup(void **state) {
    elosMessageHandlerHandleMessageUtestCreateConnection(state);
    return 0;
}

int elosTestElosMessageHandlerHandleMessageExterrFirstAllocMemTeardown(void **state) {
    elosMessageHandlerHandleMessageUtestFreeConnection(state);
    return 0;
}

void elosTestElosMessageHandlerHandleMessageExterrFirstAllocMem(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientManagerConnection_t *conn = testState->connection;
    safuResultE_t retval;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the first safuAllocMem fails");

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosMessage_t));
    will_return(__wrap_safuAllocMem, NULL);
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);

    retval = elosMessageHandlerHandleMessage(conn);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
