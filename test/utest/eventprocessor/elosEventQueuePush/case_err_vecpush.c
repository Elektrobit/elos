// SPDX-License-Identifier: MIT

#include "elosEventQueuePush_utest.h"
#include "safu/mock_vector.h"

int elosTestEloEventQueuePushErrVecPushSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueuePushErrVecPushTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    pthread_mutex_unlock(test->eventQueue.mutex);
    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueuePushErrVecPush(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t event = {0};
    safuResultE_t result;

    TEST("elosEventQueuePush (vecPush)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when vecPush fails");

    MOCK_FUNC_AFTER_CALL(safuVecPush, 0);
    expect_not_value(__wrap_safuVecPush, vec, NULL);
    expect_not_value(__wrap_safuVecPush, element, NULL);
    will_return(__wrap_safuVecPush, -1);

    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
