// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorPushByParam_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterNodeVectorPushByParamVectorNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeVectorPushByParamVectorNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeVectorPushByParamVectorNull(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since the given node vector pointer is NULL");

    char const *filterStrings[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    elosEventFilterNodeParam_t const param = {.filterStrings = filterStrings, .filterStringCount = 2};

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorPushByParam(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
