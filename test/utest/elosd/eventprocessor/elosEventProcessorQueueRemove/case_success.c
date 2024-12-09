// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueRemove_utest.h"

int elosTestEloEventProcessorQueueRemoveSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventProcessorQueueCreate(&test->eventProcessor, &test->eventQueueId);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorQueueRemoveSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorQueueRemoveSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorQueueRemove");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorQueueRemove(&test->eventProcessor, test->eventQueueId);

    assert_int_equal(result, SAFU_RESULT_OK);
}
