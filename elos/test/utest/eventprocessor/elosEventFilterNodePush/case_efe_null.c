// SPDX-License-Identifier: MIT

#include "elosEventFilterNodePush_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodePushEfeNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodePushEfeNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodePushEfeNull(UNUSED void **state) {
    char const *fStr[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    size_t fCnt = ARRAY_SIZE(fStr);
    safuResultE_t result;

    TEST("elosEventFilterNodePush");
    SHOULD("%s", "failed since given filter node pointer is NULL");

    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "Invalid parameter given");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodePush(NULL, fStr, fCnt);

    assert_true(result == SAFU_RESULT_FAILED);
}
