// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderNew_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosRpnFilterBuilder_t *data;
} _TestState_t;

int elos_testElosRpnFilterBuilderNewSuccessSetup(void **state) {
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderNewSuccess(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    const elosRpnFilterParam_t param = {.filterString = "test123"};
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderNew");
    SHOULD("%s", "Properly allocate and initialize memory");

    result = elosRpnFilterBuilderNew(&param, &test->data);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_non_null(test->data);
    assert_true(test->data->status == RPNFILTER_CREATE_INITIALIZED);
}

int elos_testElosRpnFilterBuilderNewSuccessTeardown(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
