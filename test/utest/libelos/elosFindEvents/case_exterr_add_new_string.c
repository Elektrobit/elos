// SPDX-License-Identifier: MIT

#include <safu/mock_safu.h>

#include "elosFindEvents_utest.h"

int elosTestElosFindEventsExtErrAddNewStringSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    return 0;
}

int elosTestElosFindEventsExtErrAddNewStringTeardown(void **state) {
    elosFindEventsUtestFreeSession(state);
    return 0;
}

void elosTestElosFindEventsExtErrAddNewString(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonAddNewString returns null");

    session->fd = VALID_SESSION_FD;
    session->connected = true;

    expect_not_value(__wrap_safuJsonAddNewString, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewString, name, "filter");
    expect_string(__wrap_safuJsonAddNewString, val, filterRule);
    will_return(__wrap_safuJsonAddNewString, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewString, 0);

    result = elosFindEvents(session, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
