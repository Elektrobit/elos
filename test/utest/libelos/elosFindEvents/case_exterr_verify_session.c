// SPDX-License-Identifier: MIT

#include "elosFindEvents_utest.h"

int elosTestElosFindEventsExtErrVerifySessionSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    return 0;
}

int elosTestElosFindEventsExtErrVerifySessionTeardown(void **state) {
    elosFindEventsUtestFreeSession(state);
    return 0;
}

void elosTestElosFindEventsExtErrVerifySession(void **state) {
    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when _verifySession returns a negative value");

    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    session->fd = INVALID_SESSION_FD;
    session->connected = true;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    result = elosFindEvents(session, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
