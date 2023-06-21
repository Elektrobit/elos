// SPDX-License-Identifier: MIT

#include "elosEventFilterNodePush_utest.h"
#include "mock_eventfilter.h"
#include "mock_rpnfilter.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodePushPushFailedSetup(void **state) {
    elosEventFilterNodeParam_t param = {0};
    elosTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null(test);
    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->filterString1 = ".event.source.appName 'test' STRCMP";
    test->filterString2 = "1 1 AND";
    test->param1.filterString = test->filterString1;
    elosEventFilterCreate(&test->filter1, &test->param1);

    *state = test;
    return 0;
}

int elosTestEloEventFilterNodePushPushFailedTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterDeleteMembers(&test->filter1);
    elosEventFilterNodeDeleteMembers(&test->efe);

    free(test);

    return 0;
}

void elosTestEloEventFilterNodePushPushFailed(void **state) {
    elosTestState_t *test = *state;
    char const *fStr[] = {test->filterString1, test->filterString2};
    size_t fCnt = ARRAY_SIZE(fStr);
    safuResultE_t result;

    TEST("elosEventFilterNodePush");
    SHOULD("%s", "fails since elosEventFilterVectorPush failed");

    MOCK_FUNC_ENABLE(safuLogFuncF);
    MOCK_FUNC_ENABLE(elosEventFilterCreate);
    MOCK_FUNC_ENABLE(elosRpnFilterVectorPush);

    expect_any(elosEventFilterCreate, filter);
    expect_memory(elosEventFilterCreate, param, &test->param1, sizeof(elosEventFilterParam_t));
    will_set_parameter(elosEventFilterCreate, filter, &test->filter1);
    will_return(elosEventFilterCreate, SAFU_RESULT_OK);

    expect_value(elosRpnFilterVectorPush, rpnFilterVector, &test->efe.eventFilterVector);
    expect_memory(elosRpnFilterVectorPush, rpnFilter, &test->filter1, sizeof(elosEventFilter_t));
    will_return(elosRpnFilterVectorPush, SAFU_RESULT_FAILED);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "elosEventFilterVectorPush failed");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodePush(&test->efe, fStr, fCnt);

    assert_true(result == SAFU_RESULT_FAILED);
}
