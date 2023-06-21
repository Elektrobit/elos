// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeInitialize_utest.h"

#define _FILTERID 42

int elosTestEloEventFilterNodeInitializeErrMutexSetup(UNUSED void **state) {
    *state = calloc(1, sizeof(elosTestState_t));
    assert_non_null(*state);

    return 0;
}

int elosTestEloEventFilterNodeInitializeErrMutexTeardown(UNUSED void **state) {
    elosTestState_t *test = *state;

    if (test->efe.mutex != NULL) {
        pthread_mutex_unlock(test->efe.mutex);
    }
    elosEventFilterNodeDeleteMembers(&test->efe);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeInitializeErrMutex(UNUSED void **state) {
    elosTestState_t *test = *state;
    elosEventQueue_t eventQueue = {0};
    safuResultE_t result;
    char const *fStr[] = {".event.source.appName 'test' STRCMP"};
    elosEventFilterNodeParam_t const param = {
        .filterStrings = fStr,
        .filterStringCount = ARRAY_SIZE(fStr),
        .eventQueue = &eventQueue,
        .id = _FILTERID,
    };

    TEST("elosEventFilterNodeInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_init fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_init, 0);
    expect_not_value(__wrap_pthread_mutex_init, __mutex, NULL);
    expect_value(__wrap_pthread_mutex_init, __mutexattr, NULL);
    will_return(__wrap_pthread_mutex_init, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    result = elosEventFilterNodeDeleteMembers(&test->efe);
    assert_true(result == SAFU_RESULT_OK);

    PARAM("%s", "pthread_mutex_lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    result = elosEventFilterNodeDeleteMembers(&test->efe);
    assert_true(result == SAFU_RESULT_OK);

    PARAM("%s", "pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    result = elosEventFilterNodeDeleteMembers(&test->efe);
    assert_true(result == SAFU_RESULT_OK);
}
