// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <signal.h>

#include "elosEventDispatcherInitialize_utest.h"

int testElosEventDispatcherInitializeErrMutexSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherInitializeErrMutexTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherInitializeErrMutex(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
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

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_true(result == SAFU_RESULT_OK);

    PARAM("%s", "pthread_mutex_lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_true(result == SAFU_RESULT_OK);

    PARAM("%s", "pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_true(result == SAFU_RESULT_FAILED);
    pthread_mutex_unlock(&test->eventDispatcher.lock);
}
