// SPDX-License-Identifier: MIT

#include <string.h>

#include "elosLogAggregatorFindEvents_utest.h"

int elosTestElosLogAggregatorFindEventsExterrMutexSetup(void **state) {
    elosUteststateT_t *testState;

    testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);
    memset(testState, 0, sizeof(elosUteststateT_t));

    *state = testState;
    return 0;
}

int elosTestElosLogAggregatorFindEventsExterrMutexTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState);

    return 0;
}

void elosTestElosLogAggregatorFindEventsExterrMutex(void **state) {
    const char *filterRule = ".event.source.appName 'testApp' STRCMP";
    elosUteststateT_t *test = *(elosUteststateT_t **)state;
    safuResultE_t result;

    TEST("elosEventQueueRead (mutex locking)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when mutex locking fails");

    MOCK_FUNC_ALWAYS(raise);
    will_return_always(__wrap_raise, 0);

    PARAM("%s", "pthread_mutex_lock fails");
    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_value(__wrap_pthread_mutex_lock, __mutex, test->logAggregator.lock);
    will_return(__wrap_pthread_mutex_lock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosLogAggregatorFindEvents(&test->logAggregator, filterRule, &test->eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_value(__wrap_pthread_mutex_lock, __mutex, test->logAggregator.lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_value(__wrap_pthread_mutex_unlock, __mutex, test->logAggregator.lock);
    will_return(__wrap_pthread_mutex_unlock, -1);
    expect_value(__wrap_raise, __sig, SIGTERM);

    result = elosLogAggregatorFindEvents(&test->logAggregator, filterRule, &test->eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(raise);
}
