// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"

int elosTestEloEventQueueReadErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));
    result = elosEventQueueInitialize(&test->eventQueue, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventQueueReadErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventQueueDeleteMembers(&test->eventQueue);

    return 0;
}

void elosTestEloEventQueueReadErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueue_t invalidEventQueue = {0};
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result;

    TEST("elosEventQueueRead (parameters)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventQueueRead(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventQueue, NULL");
    result = elosEventQueueRead(&test->eventQueue, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventQueue, NULL");
    result = elosEventQueueRead(&invalidEventQueue, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &eventVector");
    result = elosEventQueueRead(NULL, &eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidEventQueue, &eventVector");
    result = elosEventQueueRead(&invalidEventQueue, &eventVector);
    assert_true(result == SAFU_RESULT_FAILED);
}
