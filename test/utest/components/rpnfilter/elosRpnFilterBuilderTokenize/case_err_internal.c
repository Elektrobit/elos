// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/vector.h>

#include "elosRpnFilterBuilderTokenize_utest.h"

int elos_testElosRpnFilterBuilderTokenizeErrInternalSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeErrInternal(void **state) {
    SKIP("%s", "conditional mocking in mock safu vector not implemented yet");
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosRpnFilterParam_t param = {.filterString = "1 .1 EQ"};
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR and 0 elements in data->token");

    result = elosRpnFilterBuilderNew(&param, &test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_non_null(test->data);

    // Simulate an error with safuVecPush
    MOCK_FUNC_AFTER_CALL(safuVecPush, 0);
    expect_any(safuVecPush, vec);
    expect_any(safuVecPush, element);
    will_return(safuVecPush, -1);

    result = elosRpnFilterBuilderTokenize(test->data);
    assert_true(result == RPNFILTER_RESULT_ERROR);
    assert_true(safuVecElements(&test->data->token) == 0);
}

int elos_testElosRpnFilterBuilderTokenizeErrInternalTeardown(void **state) {
    elosTestState_t *test = (elosTestState_t *)*state;
    // elosRpnFilterBuilderDelete(test->data); enable when unskipping test or function fixed.
    free(test);
    return 0;
}
