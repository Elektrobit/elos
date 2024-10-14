// SPDX-License-Identifier: MIT

#include <safu/mock_safu.h>
#include <safu/result.h>

#include "elosFindEvents_utest.h"

int elosTestElosFindEventsExtErrAddNewTimestampSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    return 0;
}

int elosTestElosFindEventsExtErrAddNewTimestampTeardown(void **state) {
    elosFindEventsUtestFreeSession(state);
    return 0;
}

void elosTestElosFindEventsExtErrAddNewTimestamp(void **state) {
    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonAddNewTimestamp returns null");

    elosUteststateT_t *testState = *state;
    elosSession_t *session = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    elosEventVector_t *vector = NULL;

    expect_not_value(__wrap_safuJsonAddNewTimestamp, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewTimestamp, name, "newest");
    expect_memory(__wrap_safuJsonAddNewTimestamp, timestamp, &newest, sizeof(newest));
    will_return(__wrap_safuJsonAddNewTimestamp, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewTimestamp, 0);

    safuResultE_t result = elosFindEvents(session, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    expect_not_value(__wrap_safuJsonAddNewTimestamp, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewTimestamp, name, "oldest");
    expect_memory(__wrap_safuJsonAddNewTimestamp, timestamp, &oldest, sizeof(oldest));
    will_return(__wrap_safuJsonAddNewTimestamp, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewTimestamp, 1);

    result = elosFindEvents(session, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
