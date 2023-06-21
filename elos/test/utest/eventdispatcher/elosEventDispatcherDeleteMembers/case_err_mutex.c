// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <signal.h>

#include "elosEventDispatcherDeleteMembers_utest.h"

int testElosEventDispatcherDeleteMembersErrMutexSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherDeleteMembersErrMutexTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherDeleteMembersErrMutex(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };

    TEST("elosEventDispatcherDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    pthread_mutex_unlock(&test->eventDispatcher.lock);
    pthread_mutex_destroy(&test->eventDispatcher.lock);

    PARAM("%s", "pthread_mutex_destroy fails");

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_destroy, 0);
    expect_not_value(__wrap_pthread_mutex_destroy, __mutex, NULL);
    will_return(__wrap_pthread_mutex_destroy, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    pthread_mutex_destroy(&test->eventDispatcher.lock);
}
