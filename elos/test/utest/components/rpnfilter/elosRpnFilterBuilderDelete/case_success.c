// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderDelete_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosRpnFilterBuilder_t *data;
} _TestState_t;

int elos_testElosRpnFilterBuilderDeleteSuccessSetup(void **state) {
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderDeleteSuccess(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    const elosRpnFilterParam_t param = {.filterString = ".1 1 EQ"};
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderNew");
    SHOULD("%s", "Properly free all allocated memory");

    result = elosRpnFilterBuilderNew(&param, &test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_non_null(test->data);

    result = elosRpnFilterBuilderTokenize(test->data);
    assert_true(result == RPNFILTER_RESULT_OK);

    result = elosRpnFilterBuilderDelete(test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    test->data = NULL;
}

int elos_testElosRpnFilterBuilderDeleteSuccessTeardown(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
