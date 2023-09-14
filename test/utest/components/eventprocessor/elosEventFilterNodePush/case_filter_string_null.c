// SPDX-License-Identifier: MIT

#include "elosEventFilterNodePush_utest.h"
#include "mock_eventfilter.h"
#include "mock_rpnfilter.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodePushFilterStringNullSetup(void **state) {
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

int elosTestEloEventFilterNodePushFilterStringNullTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterDeleteMembers(&test->filter1);
    elosEventFilterNodeDeleteMembers(&test->efe);

    free(test);

    return 0;
}

void elosTestEloEventFilterNodePushFilterStringNull(void **state) {
    elosTestState_t *test = *state;
    char const *fStr[] = {NULL, NULL};
    size_t fCnt = ARRAY_SIZE(fStr);
    safuResultE_t result;

    TEST("elosEventFilterNodePush");
    SHOULD("%s", "fails since elosEventFilterVectorPush failed");

    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "filterString idx:0 is NULL");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodePush(&test->efe, fStr, fCnt);

    MOCK_FUNC_NEVER(safuLogFunc);

    assert_true(result == SAFU_RESULT_FAILED);
}
