// SPDX-License-Identifier: MIT

#include "elosEventQueuePush_utest.h"

int elosTestEloEventQueuePushErrParamSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueuePushErrParamTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueuePushErrParam(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueue_t invalidEventQueue = {0};
    elosEvent_t const event = {0};
    safuResultE_t result;

    TEST("elosEventQueuePush (parameters)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventQueuePush(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventQueue, NULL");
    result = elosEventQueuePush(&test->eventQueue, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventQueue, NULL");
    result = elosEventQueuePush(&invalidEventQueue, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &event");
    result = elosEventQueuePush(NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventQueue, &event");
    result = elosEventQueuePush(&invalidEventQueue, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
