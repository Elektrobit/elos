// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorPushByParam_utest.h"
#include "mock_eventfilternode.h"
#include "safu/mock_log.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorPushByParamInitializeFailedSetup(void **state) {
    safuResultE_t result;
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    result = elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorPushByParamInitializeFailedTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorPushByParamInitializeFailed(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since elosEventFilterNodeInitialize fails");

    char const *filterStrings[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    elosEventFilterNodeParam_t const param = {.filterStrings = filterStrings, .filterStringCount = 2};

    MOCK_FUNC_ENABLE(elosEventFilterNodeInitialize);
    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_any(elosEventFilterNodeInitialize, eventFilterNode);
    expect_value(elosEventFilterNodeInitialize, param, &param);
    will_return(elosEventFilterNodeInitialize, SAFU_RESULT_FAILED);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "elosEventFilterNodeInitialize failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorPushByParam(&test->efev, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(test->efev.elementCount, 0);
}
