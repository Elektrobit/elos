// SPDX-License-Identifier: MIT

#include "elosFindEvents_utest.h"

int elosTestElosFindEventsExtErrNewObjectSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    return 0;
}

int elosTestElosFindEventsExtErrNewObjectTeardown(void **state) {
    elosFindEventsUtestFreeSession(state);
    return 0;
}

void elosTestElosFindEventsExtErrNewObject(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_object_new_object returns null");

    session->fd = VALID_SESSION_FD;
    session->connected = true;

    will_return(__wrap_json_object_new_object, NULL);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    result = elosFindEvents(session, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
