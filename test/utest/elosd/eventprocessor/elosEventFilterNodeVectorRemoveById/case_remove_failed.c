// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorRemoveById_utest.h"
#include "safu/mock_log.h"
#include "safu/mock_vector.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorRemoveByIdRemoveFailedSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    const char *string1 = ".event.source.appName 'test' STRCMP";
    const char *string2 = "1 1 AND";
    char const **filterStrings1 = &string1;
    char const **filterStrings2 = &string2;
    elosEventFilterNodeParam_t const p1 = {.filterStrings = filterStrings1, .filterStringCount = 1, .id = 1};
    elosEventFilterNodeParam_t const p2 = {.filterStrings = filterStrings2, .filterStringCount = 1, .id = 2};
    elosEventFilterNodeVectorPushByParam(&test->efev, &p1);
    elosEventFilterNodeVectorPushByParam(&test->efev, &p2);

    *state = test;
    return 0;
}

int elosTestEloEventFilterNodeVectorRemoveByIdRemoveFailedTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeVectorDeleteMembers(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorRemoveByIdRemoveFailed(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail as safuVecRemove returns with non zero return code");

    MOCK_FUNC_ENABLE(safuVecRemove);
    MOCK_FUNC_ENABLE(safuLogFuncF);

    expect_value(__wrap_safuVecRemove, vec, &test->efev);
    expect_value(__wrap_safuVecRemove, idx, 0);
    will_return(__wrap_safuVecRemove, -1);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "safuVecRemove failed");
    will_return(__wrap_safuLogFuncF, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorRemoveById(&test->efev, 1);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(test->efev.elementCount, 2);
}
