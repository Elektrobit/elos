// SPDX-License-Identifier: MIT

#include "elosRpnFilterCreate_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosRpnFilter_t filter;
} _TestState_t;

int elos_testElosRpnFilterCreateSuccessSetup(void **state) {
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));
    *state = test;
    return 0;
}

int elos_testElosRpnFilterCreateSuccessTeardown(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterDeleteMembers(&test->filter);
    free(test);
    return 0;
}

void elos_testElosRpnFilterCreateSuccess(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterResultE_t result;
    const elosRpnFilterParam_t param = {.filterString = "1 .1 EQ"};

    result = elosRpnFilterCreate(&test->filter, &param);
    assert_true(result == RPNFILTER_RESULT_OK);

    // We only do a cursory check of the resulting filter here;
    // The inner works of this function (as well as the resulting filter) are tested
    // much more comprehensively with the unit tests for the elosRpnFilterBuilder* functions
    assert_false(test->filter.steps == NULL);
    assert_true(test->filter.steps->count == 3);
    assert_false(test->filter.values == NULL);
    assert_true(test->filter.values->count == 2);

    result = elosRpnFilterDeleteMembers(&test->filter);
    assert_true(result == RPNFILTER_RESULT_OK);
}
