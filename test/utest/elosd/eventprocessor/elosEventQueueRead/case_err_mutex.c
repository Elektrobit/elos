// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"

int elosTestEloEventQueueReadErrMutexSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueueReadErrMutexTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    pthread_mutex_unlock(test->eventQueue.mutex);
    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueueReadErrMutex(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result;

    TEST("elosEventQueueRead (mutex locking)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when mutex locking fails");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_lock fails");
    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventQueueRead(&test->eventQueue, &eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "pthread_mutex_unlock fails");
    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventQueueRead(&test->eventQueue, &eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(raise);
}
