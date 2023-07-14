// SPDX-License-Identifier: MIT

#include "elosEventQueuePush_utest.h"

int elosTestEloEventQueuePushErrMutexSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueuePushErrMutexTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    pthread_mutex_unlock(test->eventQueue.mutex);
    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueuePushErrMutex(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t event = {0};
    safuResultE_t result;

    TEST("elosEventQueuePush (mutex locking)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when mutex locking fails");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_lock fails");
    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "pthread_mutex_unlock fails");
    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_not_value(__wrap_pthread_mutex_unlock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(raise);
}
