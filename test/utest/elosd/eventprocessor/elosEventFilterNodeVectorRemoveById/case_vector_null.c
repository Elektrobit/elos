// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorRemoveById_utest.h"
#include "safu/mock_log.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorRemoveByIdVectorNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeVectorRemoveByIdVectorNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeVectorRemoveByIdVectorNull(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since the given vector is a null pointer");

    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "Invalid parameter");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorRemoveById(NULL, 1);
    assert_true(result == SAFU_RESULT_FAILED);
}
