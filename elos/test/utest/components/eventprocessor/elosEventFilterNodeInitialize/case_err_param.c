// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeInitialize_utest.h"

#define _FILTERID 42

int elosTestEloEventFilterNodeInitializeErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeInitializeErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeInitializeErrParam(UNUSED void **state) {
    elosTestState_t *test = *state;
    elosEventQueue_t eventQueue = {0};
    char const *fStr[] = {".event.source.appName 'test' STRCMP"};
    elosEventFilterNodeParam_t const paramOk = {
        .filterStrings = fStr, .filterStringCount = ARRAY_SIZE(fStr), .eventQueue = &eventQueue, .id = _FILTERID};
    elosEventFilterNodeParam_t const paramErr = {
        .filterStrings = NULL, .filterStringCount = 0, .eventQueue = &eventQueue, .id = _FILTERID};
    safuResultE_t result;

    TEST("elosEventFilterNodeInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    result = elosEventFilterNodeInitialize(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventFilterNodeInitialize(&test->efe, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventFilterNodeInitialize(NULL, &paramOk);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventFilterNodeInitialize(&test->efe, &paramErr);
    assert_true(result == SAFU_RESULT_FAILED);
}
