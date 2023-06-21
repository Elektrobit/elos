// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <signal.h>

#include "elosEventDispatcherStop_utest.h"

int testElosEventDispatcherStopErrMutexSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventDispatcherStart(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherStopErrMutexTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherStopErrMutex(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherStart");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherStop(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventDispatcherStop(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    pthread_mutex_unlock(&test->eventDispatcher.lock);
}
