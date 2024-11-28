// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfilternode.h"
#include "elosEventFilterNodeVectorDeleteMembers_utest.h"
#include "safu/mock_log.h"
#include "safu/mock_vector.h"

#define _VECTORELEMENTS 2

int elosTestEloEventFilterNodeVectorDeleteMembersFreeFailedSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));

    elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorDeleteMembersFreeFailedTeardown(void **state) {
    elosTestState_t *test = *state;

    safuVecFree(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorDeleteMembersFreeFailed(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorDeleteMembers");
    SHOULD("%s", "should return a failure code since safuVecFree fails");

    MOCK_FUNC_ENABLE(safuVecFind);
    MOCK_FUNC_ENABLE(safuVecFree);
    MOCK_FUNC_ENABLE(safuLogFunc);

    // necessary to mock this, since it would otherwise result in a premature call to vecFree
    // from inside the repeating delete function.
    expect_value(__wrap_safuVecFind, vec, &test->efev);
    expect_value(__wrap_safuVecFind, startIdx, 0);
    expect_any(__wrap_safuVecFind, matchFunc);
    expect_value(__wrap_safuVecFind, matchData, NULL);
    expect_value(__wrap_safuVecFind, resultIdx, NULL);
    will_return(__wrap_safuVecFind, SAFU_RESULT_OK);

    expect_value(__wrap_safuVecFree, vec, &test->efev);
    will_return(__wrap_safuVecFree, -1);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_WARN);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "safuVecFree failed (possible memory leak)");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorDeleteMembers(&test->efev);
    assert_true(result == SAFU_RESULT_FAILED);
}
