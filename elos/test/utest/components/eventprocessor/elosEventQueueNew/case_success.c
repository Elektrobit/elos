// SPDX-License-Identifier: MIT

#include "elosEventQueueNew_utest.h"

int elosTestEloEventQueueNewSuccessSetup(UNUSED void **state) {
    elosUnitTestState_t *test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    *state = test;
    return 0;
}

int elosTestEloEventQueueNewSuccessTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueDelete(test->eventQueue);
    free(test);
    return 0;
}

void elosTestEloEventQueueNewSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    TEST("elosEventQueueNew");
    SHOULD("%s", "Test successful create of new queue");

    result = elosEventQueueNew(&test->eventQueue, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventQueue->eventVector);
    assert_int_equal(test->eventQueue->eventVector->elementCount, 0);
    assert_non_null(test->eventQueue->eventVector->data);
}
