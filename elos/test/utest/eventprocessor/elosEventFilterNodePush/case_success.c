// SPDX-License-Identifier: MIT

#include <elos/eventprocessor/eventfilternode.h>

#include "elosEventFilterNodePush_utest.h"
#include "mock_eventfilter.h"
#include "mock_rpnfilter.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodePushSuccessSetup(void **state) {
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
    test->param2.filterString = test->filterString2;
    elosEventFilterCreate(&test->filter1, &test->param1);
    elosEventFilterCreate(&test->filter2, &test->param2);

    *state = test;
    return 0;
}

int elosTestEloEventFilterNodePushSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterDeleteMembers(&test->filter1);
    elosEventFilterDeleteMembers(&test->filter2);
    elosEventFilterNodeDeleteMembers(&test->efe);

    free(test);

    return 0;
}

void elosTestEloEventFilterNodePushSuccess(void **state) {
    elosTestState_t *test = *state;
    char const *fStr[] = {test->filterString1, test->filterString2};
    size_t fCnt = ARRAY_SIZE(fStr);
    safuResultE_t result;

    TEST("elosEventFilterNodePush");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodePush");

    MOCK_FUNC_ALWAYS(safuLogFuncF);
    MOCK_FUNC_ALWAYS(elosEventFilterCreate);
    MOCK_FUNC_ALWAYS(elosRpnFilterVectorPush);

    expect_any(elosEventFilterCreate, filter);
    expect_memory(elosEventFilterCreate, param, &test->param1, sizeof(elosEventFilterParam_t));
    will_set_parameter(elosEventFilterCreate, filter, &test->filter1);
    will_return(elosEventFilterCreate, SAFU_RESULT_OK);

    expect_value(elosRpnFilterVectorPush, rpnFilterVector, &test->efe.eventFilterVector);
    expect_memory(elosRpnFilterVectorPush, rpnFilter, &test->filter1, sizeof(elosEventFilter_t));
    will_return(elosRpnFilterVectorPush, SAFU_RESULT_OK);

    expect_any(elosEventFilterCreate, filter);
    expect_memory(elosEventFilterCreate, param, &test->param2, sizeof(elosEventFilterParam_t));
    will_set_parameter(elosEventFilterCreate, filter, &test->filter2);
    will_return(elosEventFilterCreate, SAFU_RESULT_OK);

    expect_value(elosRpnFilterVectorPush, rpnFilterVector, &test->efe.eventFilterVector);
    expect_memory(elosRpnFilterVectorPush, rpnFilter, &test->filter2, sizeof(elosEventFilter_t));
    will_return(elosRpnFilterVectorPush, SAFU_RESULT_OK);

    result = elosEventFilterNodePush(&test->efe, fStr, fCnt);

    MOCK_FUNC_NEVER(safuLogFunc);
    MOCK_FUNC_NEVER(elosEventFilterCreate);
    MOCK_FUNC_NEVER(elosRpnFilterVectorPush);

    assert_true(result == SAFU_RESULT_OK);
}
