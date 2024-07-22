// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosRpnFilterBuilder_t *data;
} _TestState_t;

int elos_testElosRpnFilterBuilderCompileErrInvalidFilterSetup(UNUSED void **state) {
    const elosRpnFilterParam_t param = {.filterString = "jsdlfjskldjfklsd"};
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));

    elosRpnFilterBuilderNew(&param, &(test->data));
    elosRpnFilterBuilderTokenize(test->data);
    *state = test;

    return 0;
}

void elos_testElosRpnFilterBuilderCompileErrInvalidFilter(UNUSED void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "faile to compile invalid filter rule");

    result = elosRpnFilterBuilderCompile(test->data);

    assert_true(result == RPNFILTER_RESULT_ERROR);
    assert_true(test->data->status == RPNFILTER_CREATE_TOKENIZED);
}

int elos_testElosRpnFilterBuilderCompileErrInvalidFilterTeardown(UNUSED void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
