// SPDX-License-Identifier: MIT

#include "elosEventProcessorDeleteMembers_utest.h"
#include "mock_eventfiltermanager.h"
#include "mock_eventqueuemanager.h"

int elosTestEloEventProcessorDeleteMembersErrComponentSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorDeleteMembersErrComponentTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorDeleteMembersErrComponent(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorDeleteMembers (failed component)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after a component failed to free up memory");

    PARAM("%s", "failed elosEventFilterManagerDeleteMembers");
    MOCK_FUNC_AFTER_CALL(elosEventFilterManagerDeleteMembers, 0);
    expect_not_value(elosEventFilterManagerDeleteMembers, efm, NULL);
    will_return(elosEventFilterManagerDeleteMembers, SAFU_RESULT_FAILED);

    result = elosEventProcessorDeleteMembers(&test->eventProcessor);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "failed elosEventQueueManagerDeleteMembers");
    MOCK_FUNC_AFTER_CALL(elosEventQueueManagerDeleteMembers, 0);
    expect_not_value(elosEventQueueManagerDeleteMembers, eqm, NULL);
    will_return(elosEventQueueManagerDeleteMembers, SAFU_RESULT_FAILED);

    result = elosEventProcessorDeleteMembers(&test->eventProcessor);
    assert_true(result == SAFU_RESULT_FAILED);
}
