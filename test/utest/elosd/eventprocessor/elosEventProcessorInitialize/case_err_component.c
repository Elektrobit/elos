// SPDX-License-Identifier: MIT

#include "elosEventProcessorInitialize_utest.h"
#include "mock_eventfiltermanager.h"
#include "mock_eventqueuemanager.h"

int elosTestEloEventProcessorInitializeErrComponentSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    memset(test, 0, sizeof(elosTestState_t));
    return 0;
}

int elosTestEloEventProcessorInitializeErrComponentTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorInitializeErrComponent(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    TEST("elosEventProcessorInitialize (failed component)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after a component failed to initialize");

    PARAM("%s", "failed elosEventQueueManagerInitialize");
    MOCK_FUNC_AFTER_CALL(elosEventQueueManagerInitialize, 0);
    expect_not_value(elosEventQueueManagerInitialize, eqm, NULL);
    expect_any(elosEventQueueManagerInitialize, param);
    will_return(elosEventQueueManagerInitialize, SAFU_RESULT_FAILED);

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "failed elosEventFilterManagerInitialize");
    MOCK_FUNC_AFTER_CALL(elosEventFilterManagerInitialize, 0);
    expect_not_value(elosEventFilterManagerInitialize, efm, NULL);
    expect_any(elosEventFilterManagerInitialize, param);
    will_return(elosEventFilterManagerInitialize, SAFU_RESULT_FAILED);

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_FAILED);
}
