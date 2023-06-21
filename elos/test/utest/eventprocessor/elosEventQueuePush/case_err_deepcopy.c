// SPDX-License-Identifier: MIT

#include "elosEventQueuePush_utest.h"
#include "mock_event.h"

int elosTestEloEventQueuePushErrDeepCopySetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueuePushErrDeepCopyTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    pthread_mutex_unlock(test->eventQueue.mutex);
    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueuePushErrDeepCopy(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t event = {0};
    safuResultE_t result;

    TEST("elosEventQueuePush (eventDeepcopy)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventDeepCopy fails");

    MOCK_FUNC_AFTER_CALL(elosEventDeepCopy, 0);
    expect_not_value(elosEventDeepCopy, to, NULL);
    expect_not_value(elosEventDeepCopy, from, NULL);
    will_return(elosEventDeepCopy, SAFU_RESULT_FAILED);

    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
