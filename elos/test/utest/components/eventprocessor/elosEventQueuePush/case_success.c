// SPDX-License-Identifier: MIT

#include "elosEventQueuePush_utest.h"

int elosTestEloEventQueuePushSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueuePushSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueuePushSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t event = {0};
    elosEvent_t *element;
    safuResultE_t result;

    TEST("elosEventQueuePush");
    SHOULD("%s", "Test successful code execution");

    PARAM("%s", "push first event");
    event.messageCode = ELOS_MSG_CODE_DEBUG_LOG;
    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->eventQueue.eventVector->elementCount == 1);
    element = safuVecGetLast(test->eventQueue.eventVector);
    assert_non_null(element);
    assert_true(element->messageCode == event.messageCode);

    PARAM("%s", "push second event");
    event.messageCode = ELOS_MSG_CODE_INFO_LOG;
    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->eventQueue.eventVector->elementCount == 2);
    element = safuVecGetLast(test->eventQueue.eventVector);
    assert_non_null(element);
    assert_true(element->messageCode == event.messageCode);
}
