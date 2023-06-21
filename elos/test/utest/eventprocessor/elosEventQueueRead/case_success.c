// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"

int elosTestEloEventQueueReadSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueueReadSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventQueueDeleteMembers(&test->eventQueue);
    elosEventVectorDelete(test->eventVector);

    return 0;
}

void elosTestEloEventQueueReadSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t *element = NULL;
    elosEvent_t event = {0};
    safuResultE_t result;

    TEST("elosEventQueueRead");
    SHOULD("%s", "Test successful code execution");

    PARAM("%s", "read when eventQueue has events stored");
    event.messageCode = ELOS_MSG_CODE_WILDCARD_7XXX;
    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventQueueRead(&test->eventQueue, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventQueue.eventVector);
    assert_int_equal(test->eventQueue.eventVector->elementCount, 0);
    assert_non_null(test->eventQueue.eventVector->data);
    assert_non_null(test->eventVector);
    assert_int_equal(test->eventVector->elementCount, 1);
    element = safuVecGetLast(test->eventVector);
    assert_non_null_msg(element, "event vector is unexpectedly still null after reading queue");
    assert_true(element->messageCode == event.messageCode);

    elosEventVectorDelete(test->eventVector);
    test->eventVector = NULL;

    PARAM("%s", "read when eventQueue has no events stored");
    result = elosEventQueueRead(&test->eventQueue, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventQueue.eventVector);
    assert_non_null(test->eventQueue.eventVector->data);
    assert_null(test->eventVector);

    PARAM("%s", "read when eventQueue has events stored again");
    event.messageCode = ELOS_MSG_CODE_WILDCARD_6XXX;
    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventQueueRead(&test->eventQueue, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventQueue.eventVector);
    assert_int_equal(test->eventQueue.eventVector->elementCount, 0);
    assert_non_null(test->eventQueue.eventVector->data);
    assert_non_null_msg(test->eventVector, "event vector is unexpectedly still null after reading queue");
    assert_int_equal(test->eventVector->elementCount, 1);
    element = safuVecGetLast(test->eventVector);
    assert_non_null(element);
    assert_true(element->messageCode == event.messageCode);

    elosEventVectorDelete(test->eventVector);
    test->eventVector = NULL;
}
