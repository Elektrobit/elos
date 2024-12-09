// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorRemoveById_utest.h"
#include "safu/mock_log.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorRemoveByIdInvalidIdSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    *state = test;
    return 0;
}

int elosTestEloEventFilterNodeVectorRemoveByIdInvalidIdTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorRemoveByIdInvalidId(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since the given id is invalid");

    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "Invalid parameter");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorRemoveById(&test->efev, 0);
    assert_true(result == SAFU_RESULT_FAILED);
}
