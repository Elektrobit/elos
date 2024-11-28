// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorPushByParam_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorPushByParamSuccessSetup(void **state) {
    safuResultE_t result;
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    result = elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorPushByParamSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorPushByParamSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeVectorInitialize");

    char const *filterStrings[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    elosEventFilterNodeParam_t const param = {.filterStrings = filterStrings, .filterStringCount = 2};

    result = elosEventFilterNodeVectorPushByParam(&test->efev, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->efev.elementCount, 1);
    assert_int_equal(test->efev.elementSize, sizeof(elosEventFilterNode_t));
    assert_int_equal(test->efev.memorySize, sizeof(elosEventFilterNode_t) * _VECTORELEMENTS);
}
