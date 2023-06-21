// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueRead_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventProcessorQueueReadSuccessSetup(void **state) {
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

int elosTestEloEventProcessorQueueReadSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    elosEventVectorDelete(test->eventVector);
    return 0;
}

void elosTestEloEventProcessorQueueReadSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t *element;
    safuResultE_t result;

    TEST("elosEventProcessorQueueRead");
    SHOULD("%s", "Test successful code execution");

    result = elosEventProcessorQueueRead(&test->eventProcessor, test->eventQueueId, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventVector);
    assert_int_equal(test->eventVector->elementCount, 1);
    element = safuVecGetLast(test->eventVector);
    assert_non_null(element);
    assert_true(element->messageCode == ELOS_MSG_CODE_WILDCARD_8XXX);
}
