// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventExtErrVerifySessionSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventExtErrVerifySessionTeardown(void **state) {
    elosLogFindEventUtestFreeSession(state);
    return 0;
}

void elosTestElosLogFindEventExtErrVerifySession(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when _verifySession returns a negative value");

    session->fd = INVALID_SESSION_FD;
    session->connected = true;

    result = elosLogFindEvent(session, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
