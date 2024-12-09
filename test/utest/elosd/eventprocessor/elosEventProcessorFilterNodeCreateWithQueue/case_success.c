// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreateWithQueue_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventProcessorFilterNodeCreateWithQueueSuccessSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateWithQueueSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateWithQueueSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterStrings[] = {"1 1 AND", "2 1 OR"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeCreateWithQueue");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventProcessor.eventQueueManager.eventQueueVector.elementCount, 1);
    assert_int_equal(test->eventProcessor.eventFilterManager.eventFilterNodeVector.elementCount, 1);
    assert_int_not_equal(eventFilterNodeId, ELOS_ID_INVALID);
    assert_int_not_equal(eventQueueId, ELOS_ID_INVALID);
}
