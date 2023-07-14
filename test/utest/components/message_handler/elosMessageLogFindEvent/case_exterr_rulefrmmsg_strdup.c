// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosMessageLogFindEvent_utest.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgStrdupSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgStrdupTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrRuleFrmMsgStrdup(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientManagerConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when strdup in _getFilterRuleFromJsonMessage fails");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, ".event.name 'sshd' STRCMP");
    will_return(__wrap_strdup, NULL);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}