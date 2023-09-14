// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreateWithQueue_utest.h"
#include "mock_eventfiltermanager.h"
#include "mock_eventqueuemanager.h"

int elosTestEloEventProcessorFilterNodeCreateWithQueueErrComponentSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateWithQueueErrComponentTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateWithQueueErrComponent(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterStrings[] = {"1 1 AND", "2 1 OR"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeCreateWithQueue (failed component)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after a component failed to free up memory");

    PARAM("%s", "failed elosEventQueueManagerEntryNew");
    MOCK_FUNC_AFTER_CALL(elosEventQueueManagerEntryNew, 0);
    expect_not_value(elosEventQueueManagerEntryNew, eqm, NULL);
    expect_not_value(elosEventQueueManagerEntryNew, eqId, NULL);
    will_return(elosEventQueueManagerEntryNew, SAFU_RESULT_FAILED);

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &eventQueueId, &eventFilterNodeId);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "failed elosEventFilterManagerNodeCreate");
    MOCK_FUNC_AFTER_CALL(elosEventFilterManagerNodeCreate, 0);
    expect_not_value(elosEventFilterManagerNodeCreate, efm, NULL);
    expect_not_value(elosEventFilterManagerNodeCreate, filterStrings, NULL);
    expect_value(elosEventFilterManagerNodeCreate, filterStringCount, filterStringCount);
    expect_not_value(elosEventFilterManagerNodeCreate, eqId, ELOS_ID_INVALID);
    expect_not_value(elosEventFilterManagerNodeCreate, efnId, NULL);
    will_set_parameter(elosEventFilterManagerNodeCreate, efnId, ELOS_ID_INVALID);
    will_return(elosEventFilterManagerNodeCreate, SAFU_RESULT_FAILED);

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &eventQueueId, &eventFilterNodeId);
    assert_true(result == SAFU_RESULT_FAILED);
}
