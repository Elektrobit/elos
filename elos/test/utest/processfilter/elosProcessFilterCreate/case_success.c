// SPDX-License-Identifier: MIT

#include "elosProcessFilterCreate_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosProcessFilter_t filter;
} _TestState_t;

int elosTestEloProcessFilterCreateSuccessSetup(void **state) {
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));
    *state = test;
    memset(test, 0, sizeof(_TestState_t));
    return 0;
}

int elosTestEloProcessFilterCreateSuccessTeardown(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosProcessFilterDeleteMembers(&test->filter);
    free(test);
    return 0;
}

void elosTestEloProcessFilterCreateSuccess(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterResultE_t result;
    const elosProcessFilterParam_t param = {.filterString = "1 .process.uid EQ"};

    result = elosProcessFilterCreate(&test->filter, &param);
    assert_true(result == RPNFILTER_RESULT_OK);

    // We only do a cursory check of the resulting filter here;
    // The inner works of this function (as well as the resulting filter) are tested
    // much more comprehensively with the unit tests for the elosProcessFilterBuilder* functions
    assert_false(test->filter.steps == NULL);
    assert_true(test->filter.steps->count == 3);
    assert_false(test->filter.values == NULL);
    assert_true(test->filter.values->count == 2);

    result = elosProcessFilterDeleteMembers(&test->filter);
    assert_true(result == RPNFILTER_RESULT_OK);
}
