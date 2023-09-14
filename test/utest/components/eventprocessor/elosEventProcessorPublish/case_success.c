// SPDX-License-Identifier: MIT

#include "elosEventProcessorPublish_utest.h"

int elosTestEloEventProcessorPublishSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventNew(&test->event);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorPublishSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDelete(test->event);
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorPublishSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorPublish");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorPublish(&test->eventProcessor, test->event);

    assert_int_equal(result, SAFU_RESULT_OK);
}
