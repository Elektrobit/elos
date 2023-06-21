// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreateWithQueue_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventProcessorFilterNodeCreateWithQueueErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateWithQueueErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateWithQueueErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventProcessor_t eventProcessor = {0};
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;

    TEST("elosEventProcessorFilterNodeCreateWithQueue (invalid parameters)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    // We do not need to test filterStrings and filterStringCount here,
    // as these will be handled by their respective function call that has its own unit test
    PARAM("%s", "NULL, NULL, 0, NULL, NULL");
    result = elosEventProcessorFilterNodeCreateWithQueue(NULL, NULL, 0, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, NULL, 0, &eventQueueId, NULL");
    result = elosEventProcessorFilterNodeCreateWithQueue(NULL, NULL, 0, &eventQueueId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, NULL, 0, NULL, &eventFilterNodeId");
    result = elosEventProcessorFilterNodeCreateWithQueue(NULL, NULL, 0, NULL, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, NULL, 0, &eventQueueId, &eventFilterNodeId");
    result = elosEventProcessorFilterNodeCreateWithQueue(NULL, NULL, 0, &eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, NULL, 0, NULL, NULL");
    result = elosEventProcessorFilterNodeCreateWithQueue(&eventProcessor, NULL, 0, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, NULL, 0, &eventQueueId, NULL");
    result = elosEventProcessorFilterNodeCreateWithQueue(&eventProcessor, NULL, 0, &eventQueueId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, NULL, 0, NULL, &eventFilterNodeId");
    result = elosEventProcessorFilterNodeCreateWithQueue(&eventProcessor, NULL, 0, NULL, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
