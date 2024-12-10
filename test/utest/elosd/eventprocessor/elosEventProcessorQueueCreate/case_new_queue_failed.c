// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueCreate_utest.h"
#include "mock_eventqueuemanager.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorQueueCreateNewQueueFailedSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorQueueCreateNewQueueFailedTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorQueueCreateNewQueueFailed(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorQueueCreate");
    SHOULD("%s", "fail as elosEventQueueManagerEntryNew failes");

    MOCK_FUNC_ENABLE(elosEventQueueManagerEntryNew);
    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(elosEventQueueManagerEntryNew, eqm, &test->eventProcessor.eventQueueManager);
    expect_value(elosEventQueueManagerEntryNew, eqId, &test->eventQueueId);
    will_return(elosEventQueueManagerEntryNew, SAFU_RESULT_FAILED);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "elosEventQueueManagerEntryNew failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorQueueCreate(&test->eventProcessor, &test->eventQueueId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(test->eventProcessor.eventQueueManager.idCount, 0);
}
