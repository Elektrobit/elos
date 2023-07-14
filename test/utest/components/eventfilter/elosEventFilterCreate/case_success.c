// SPDX-License-Identifier: MIT

#include "elosEventFilterCreate_utest.h"
#include "safu/common.h"

typedef struct _TestState {
    elosEventFilter_t filter;
} _TestState_t;

int elosTestEloEventFilterCreateSuccessSetup(void **state) {
    _TestState_t *test = safuAllocMem(NULL, sizeof(_TestState_t));
    *state = test;
    return 0;
}

int elosTestEloEventFilterCreateSuccessTeardown(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosEventFilterDeleteMembers(&test->filter);
    free(test);
    return 0;
}

void elosTestEloEventFilterCreateSuccess(void **state) {
    _TestState_t *test = *(_TestState_t **)state;
    elosRpnFilterResultE_t result;
    const elosEventFilterParam_t param = {.filterString = "1 .event.classification EQ"};

    result = elosEventFilterCreate(&test->filter, &param);
    assert_true(result == RPNFILTER_RESULT_OK);

    // We only do a cursory check of the resulting filter here;
    // The inner works of this function (as well as the resulting filter) are tested
    // much more comprehensively with the unit tests for the elosEventFilterBuilder* functions
    assert_false(test->filter.steps == NULL);
    assert_true(test->filter.steps->count == 3);
    assert_false(test->filter.values == NULL);
    assert_true(test->filter.values->count == 2);

    result = elosEventFilterDeleteMembers(&test->filter);
    assert_true(result == RPNFILTER_RESULT_OK);
}
