// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreate_utest.h"
#include "mock_eventfiltermanager.h"

int elosTestEloEventProcessorFilterNodeCreateSuccessSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;
    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    free(test);
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterStrings[] = {"1 1 AND", "2 1 OR"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeCreate");
    SHOULD("%s", "Test successful code execution");

    MOCK_FUNC_ENABLE(elosEventFilterManagerNodeCreate);

    expect_value(elosEventFilterManagerNodeCreate, efm, &test->eventProcessor);
    expect_value(elosEventFilterManagerNodeCreate, filterStrings, filterStrings);
    expect_value(elosEventFilterManagerNodeCreate, filterStringCount, filterStringCount);
    expect_value(elosEventFilterManagerNodeCreate, eqId, eventQueueId);
    expect_value(elosEventFilterManagerNodeCreate, efnId, &eventFilterNodeId);
    will_set_parameter(elosEventFilterManagerNodeCreate, efnId, &eventFilterNodeId);
    will_return(elosEventFilterManagerNodeCreate, SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeCreate(&test->eventProcessor, filterStrings, filterStringCount, eventQueueId,
                                                &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);
}
