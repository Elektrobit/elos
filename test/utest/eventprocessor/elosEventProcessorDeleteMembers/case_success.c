// SPDX-License-Identifier: MIT

#include "elosEventProcessorDeleteMembers_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventProcessorDeleteMembersSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorDeleteMembersSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorDeleteMembers");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorDeleteMembers(&test->eventProcessor);
    assert_true(result == SAFU_RESULT_OK);
}
