// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueRead_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventProcessorQueueReadErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorQueueReadErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorQueueReadErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventProcessor_t eventProcessor = {0};
    elosEventQueueId_t invalidId = ELOS_ID_INVALID;
    elosEventQueueId_t id = 42;
    elosEventVector_t *eventVector = NULL;

    TEST("elosEventProcessorQueueRead (invalid parameters)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, invalidId, NULL");
    result = elosEventProcessorQueueRead(NULL, invalidId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, id, NULL");
    result = elosEventProcessorQueueRead(NULL, id, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, invalidId, &eventVector");
    result = elosEventProcessorQueueRead(NULL, invalidId, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, id, &eventVector");
    result = elosEventProcessorQueueRead(NULL, id, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, invalidId, NULL");
    result = elosEventProcessorQueueRead(&eventProcessor, invalidId, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, id, NULL");
    result = elosEventProcessorQueueRead(&eventProcessor, id, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, invalidId, &eventVector");
    result = elosEventProcessorQueueRead(&eventProcessor, invalidId, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
