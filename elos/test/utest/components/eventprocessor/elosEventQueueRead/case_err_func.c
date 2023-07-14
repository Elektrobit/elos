// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"
#include "mock_event_vector.h"

int elosTestEloEventQueueReadErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    elosEvent_t event = {.messageCode = ELOS_MSG_CODE_WILDCARD_7XXX};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventQueuePush(&test->eventQueue, &event);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueueReadErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    pthread_mutex_unlock(test->eventQueue.mutex);
    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueueReadErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result;

    TEST("elosEventQueueRead (function fail)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when subfunction fails");

    PARAM("%s", "elosEventVectorNew fails");
    MOCK_FUNC_AFTER_CALL(elosEventVectorNew, 0);
    expect_not_value(elosEventVectorNew, eventVector, NULL);
    expect_not_value(elosEventVectorNew, elements, 0);
    will_set_parameter(elosEventVectorNew, eventVector, NULL);
    will_return(elosEventVectorNew, SAFU_RESULT_FAILED);

    result = elosEventQueueRead(&test->eventQueue, &eventVector);
    assert_true(result == SAFU_RESULT_FAILED);
}
