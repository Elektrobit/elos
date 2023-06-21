// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/vector.h>

#include "elosRpnFilterBuilderTokenize_utest.h"

int elos_testElosRpnFilterBuilderTokenizeErrParamSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeErrParam(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    const elosRpnFilterParam_t param = {.filterString = NULL};
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Fail with RPNFILTER_RESULT_ERROR and 0 elements in data->token");

    result = elosRpnFilterBuilderNew(&param, &test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_non_null(test->data);

    result = elosRpnFilterBuilderTokenize(NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);
    assert_true(safuVecElements(&test->data->token) == 0);

    result = elosRpnFilterBuilderTokenize(test->data);
    assert_true(result == RPNFILTER_RESULT_ERROR);
    assert_true(safuVecElements(&test->data->token) == 0);

    memset(&test->data->param, 0x00, sizeof(elosRpnFilterParam_t));
    result = elosRpnFilterBuilderTokenize(test->data);
    assert_true(result == RPNFILTER_RESULT_ERROR);
    assert_true(safuVecElements(&test->data->token) == 0);
}

int elos_testElosRpnFilterBuilderTokenizeErrParamTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
