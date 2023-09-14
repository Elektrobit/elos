// SPDX-License-Identifier: MIT

#include "elosEventFilterNodePush_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodePushCountZeroSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosTestState_t));

    return 0;
}

int elosTestEloEventFilterNodePushCountZeroTeardown(void **state) {
    elosTestState_t *test = *state;

    free(test);

    return 0;
}

void elosTestEloEventFilterNodePushCountZero(void **state) {
    elosTestState_t *test = *state;
    char const *fStr[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    safuResultE_t result;

    TEST("elosEventFilterNodePush");
    SHOULD("%s", "failed since the filter string lenght parameter is zero");

    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "Invalid parameter given");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodePush(&test->efe, fStr, 0);

    assert_true(result == SAFU_RESULT_FAILED);
}
