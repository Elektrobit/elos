// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventErrConnectionSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventErrConnectionTeardown(void **state) {
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventErrConnection(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the connection parameter is null");

    result = elosMessageLogFindEvent(NULL, testState->msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}