// SPDX-License-Identifier: MIT

#include "elosEventFilterExecute_utest.h"

int elosTestEloEventFilterExecuteErrParamSetup(UNUSED void **state) {
    return 0;
}

static void _runCheck(elosEventFilter_t *filter, elosRpnFilterStack_t *param, elosEvent_t *event) {
    elosRpnFilterResultE_t result;
    result = elosEventFilterExecute(filter, param, event);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

void elosTestEloEventFilterExecuteErrParam(UNUSED void **state) {
    elosEventFilter_t filter = {0};
    elosEvent_t input = {0};

    TEST("elosEventFilterExecute");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR because of empty or NULL filter");

    ELOS_FILTERSTACK_NEW(param, 1)

    _runCheck(&filter, NULL, NULL);
    _runCheck(NULL, param, NULL);
    _runCheck(NULL, NULL, &input);
    _runCheck(NULL, NULL, NULL);
    _runCheck(&filter, param, NULL);
    _runCheck(&filter, NULL, &input);
    _runCheck(NULL, param, &input);
    _runCheck(&filter, param, &input);
}

int elosTestEloEventFilterExecuteErrParamTeardown(UNUSED void **state) {
    return 0;
}
