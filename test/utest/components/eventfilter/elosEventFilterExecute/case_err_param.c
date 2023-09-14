// SPDX-License-Identifier: MIT

#include "elosEventFilterExecute_utest.h"

int elosTestEloEventFilterExecuteErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterExecuteErrParam(UNUSED void **state) {
    elosEventFilter_t filter = {0};
    elosRpnFilterResultE_t result;
    elosEvent_t input = {0};

    TEST("elosEventFilterExecute");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR");

    ELOS_FILTERSTACK_NEW(param, 1)

    result = elosEventFilterExecute(&filter, NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosEventFilterExecute(NULL, param, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosEventFilterExecute(NULL, NULL, &input);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosEventFilterExecute(NULL, NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloEventFilterExecuteErrParamTeardown(UNUSED void **state) {
    return 0;
}
