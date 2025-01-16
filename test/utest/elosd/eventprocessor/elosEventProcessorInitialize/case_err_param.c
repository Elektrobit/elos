// SPDX-License-Identifier: MIT

#include "elosEventProcessorInitialize_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventProcessorInitializeErrParamSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    memset(test, 0, sizeof(elosTestState_t));
    return 0;
}

int elosTestEloEventProcessorInitializeErrParamTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorInitializeErrParam(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorParam_t const invalidParam = {.config = NULL};
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    TEST("elosEventProcessorInitialize (invalid parameters)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventProcessorInitialize(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, NULL");
    result = elosEventProcessorInitialize(&test->eventProcessor, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &param");
    result = elosEventProcessorInitialize(NULL, &param);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &invalidParam");
    result = elosEventProcessorInitialize(NULL, &invalidParam);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventProcessor, &invalidParam");
    result = elosEventProcessorInitialize(&test->eventProcessor, &invalidParam);
    assert_true(result == SAFU_RESULT_FAILED);
}
