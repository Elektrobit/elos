// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueCreate_utest.h"

int elosTestEloEventProcessorQueueCreateSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorQueueCreateSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorQueueCreateSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorQueueCreate");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorQueueCreate(&test->eventProcessor, &test->eventQueueId);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventProcessor.eventQueueManager.idCount, 1);
}
