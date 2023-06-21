// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventErrVectorSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventErrVectorTeardown(void **state) {
    elosLogFindEventUtestFreeSession(state);
    return 0;
}

void elosTestElosLogFindEventErrVector(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    safuResultE_t result;

    session->connected = true;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the vector parameter is null");

    result = elosLogFindEvent(session, filterRule, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
