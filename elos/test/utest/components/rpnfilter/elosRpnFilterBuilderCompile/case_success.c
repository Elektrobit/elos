// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosRpnFilterBuilder_t *data;
} _TestState_t;

int elos_testElosRpnFilterBuilderCompileSuccessSetup(UNUSED void **state) {
    const elosRpnFilterParam_t param = {.filterString = "1 .1 EQ"};
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));

    elosRpnFilterBuilderNew(&param, &(test->data));
    elosRpnFilterBuilderTokenize(test->data);
    *state = test;

    return 0;
}

void elos_testElosRpnFilterBuilderCompileSuccess(UNUSED void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosRpnFilterBuilderCompile(test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_true(test->data->status == RPNFILTER_CREATE_COMPILED);
}

int elos_testElosRpnFilterBuilderCompileSuccessTeardown(UNUSED void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
