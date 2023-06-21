// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"
#include "safu/mock_safu.h"

int elosTestElosLogFindEventExtErrAddNewStringSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventExtErrAddNewStringTeardown(void **state) {
    elosLogFindEventUtestFreeSession(state);
    return 0;
}

void elosTestElosLogFindEventExtErrAddNewString(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonAddNewString returns null");

    session->fd = VALID_SESSION_FD;
    session->connected = true;

    expect_not_value(__wrap_safuJsonAddNewString, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewString, name, "filter");
    expect_string(__wrap_safuJsonAddNewString, val, filterRule);
    will_return(__wrap_safuJsonAddNewString, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewString, 0);

    result = elosLogFindEvent(session, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
