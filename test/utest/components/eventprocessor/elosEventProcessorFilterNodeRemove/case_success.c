// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeRemove_utest.h"
#include "mock_eventfiltermanager.h"

int elosTestEloEventProcessorFilterNodeRemoveSuccessSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;
    return 0;
}

int elosTestEloEventProcessorFilterNodeRemoveSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    free(test);
    return 0;
}

void elosTestEloEventProcessorFilterNodeRemoveSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeRemove");
    SHOULD("%s", "Test successful code execution");

    MOCK_FUNC_ENABLE(elosEventFilterManagerNodeRemove);

    expect_value(elosEventFilterManagerNodeRemove, efm, &test->eventProcessor);
    expect_value(elosEventFilterManagerNodeRemove, efnId, eventFilterNodeId);
    will_return(elosEventFilterManagerNodeRemove, SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeRemove(&test->eventProcessor, eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);
}
