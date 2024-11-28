// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueRead_utest.h"
#include "mock_eventqueue.h"
#include "mock_eventqueuemanager.h"

int elosTestEloEventProcessorQueueReadErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    char const *filterStrings[] = {".event.messageCode 8000 EQ"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEvent_t event = {.messageCode = ELOS_MSG_CODE_WILDCARD_8XXX};
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeCreateWithQueue(&test->eventProcessor, filterStrings, filterStringCount,
                                                         &test->eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventProcessorPublish(&test->eventProcessor, &event);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorQueueReadErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    elosEventVectorDelete(test->eventVector);
    return 0;
}

void elosTestEloEventProcessorQueueReadErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorQueueRead (failed component)");
    SHOULD("%s", "return SAFU_RESULT_FAILED when subfunction fails");

    PARAM("%s", "failed elosEventQueueManagerEntryGet");
    MOCK_FUNC_AFTER_CALL(elosEventQueueManagerEntryGet, 0);
    expect_not_value(elosEventQueueManagerEntryGet, eqm, NULL);
    expect_not_value(elosEventQueueManagerEntryGet, eqId, ELOS_ID_INVALID);
    expect_not_value(elosEventQueueManagerEntryGet, eventQueue, NULL);
    will_set_parameter(elosEventQueueManagerEntryGet, eventQueue, NULL);
    will_return(elosEventQueueManagerEntryGet, SAFU_RESULT_FAILED);

    result = elosEventProcessorQueueRead(&test->eventProcessor, test->eventQueueId, &test->eventVector);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "failed elosEventQueueRead");
    MOCK_FUNC_AFTER_CALL(elosEventQueueRead, 0);
    expect_not_value(elosEventQueueRead, eventQueue, NULL);
    expect_not_value(elosEventQueueRead, eventVector, NULL);
    will_set_parameter(elosEventQueueRead, eventVector, NULL);
    will_return(elosEventQueueRead, SAFU_RESULT_FAILED);

    result = elosEventProcessorQueueRead(&test->eventProcessor, test->eventQueueId, &test->eventVector);
    assert_true(result == SAFU_RESULT_FAILED);
}
