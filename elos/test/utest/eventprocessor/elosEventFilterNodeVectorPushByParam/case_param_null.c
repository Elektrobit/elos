// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorPushByParam_utest.h"
#include "safu/mock_log.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorPushByParamParamNullSetup(void **state) {
    safuResultE_t result;
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    result = elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorPushByParamParamNullTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorPushByParamParamNull(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since the given Filter Node Parameter pointer is NULL");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorPushByParam(&test->efev, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
