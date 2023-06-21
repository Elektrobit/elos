// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventExtErrNewObjectSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventExtErrNewObjectTeardown(void **state) {
    elosLogFindEventUtestFreeSession(state);
    return 0;
}

void elosTestElosLogFindEventExtErrNewObject(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_object_new_object returns null");

    session->fd = VALID_SESSION_FD;
    session->connected = true;

    will_return(__wrap_json_object_new_object, NULL);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    result = elosLogFindEvent(session, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
