// SPDX-License-Identifier: MIT

#include "elosFindEvents_utest.h"

int elosTestElosFindEventsErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosFindEventsErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosFindEventsErrInvalidParameter(UNUSED void **state) {
    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when any of the parameters is null");

    elosSession_t session = {.connected = true};
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    PARAM("'session' is NULL");
    result = elosFindEvents(NULL, filterRule, &newest, &oldest, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("'filterRule' is NULL");
    result = elosFindEvents(&session, NULL, &newest, &oldest, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("'newest' is NULL");
    result = elosFindEvents(&session, filterRule, NULL, &oldest, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    PARAM("'oldest' is NULL");
    result = elosFindEvents(&session, filterRule, &newest, NULL, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    PARAM("'eventVector' is NULL");
    result = elosFindEvents(&session, filterRule, &newest, &oldest, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
