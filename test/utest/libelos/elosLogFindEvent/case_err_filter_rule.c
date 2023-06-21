// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventErrFilterRuleSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventErrFilterRuleTeardown(void **state) {
    elosLogFindEventUtestFreeSession(state);
    return 0;
}

void elosTestElosLogFindEventErrFilterRule(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    session->connected = true;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the filterRule parameter is null");

    result = elosLogFindEvent(session, NULL, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
