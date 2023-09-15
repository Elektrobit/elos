// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventErrMessageSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);

    return 0;
}

int elosTestElosMessageLogFindEventErrMessageTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);

    return 0;
}

void elosTestElosMessageLogFindEventErrMessage(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the message parameter is null");

    result = elosMessageLogFindEvent(testState->connection, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}