// SPDX-License-Identifier: MIT

#include "elosProcessFilterExecute_utest.h"

int elosTestEloProcessFilterExecuteErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloProcessFilterExecuteErrParam(UNUSED void **state) {
    elosProcessFilter_t filter = {0};
    elosRpnFilterResultE_t result;
    elosProcessIdentity_t input = {0};

    TEST("elosProcessFilterExecute");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR");

    ELOS_FILTERSTACK_NEW(param, 1)

    result = elosProcessFilterExecute(&filter, NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosProcessFilterExecute(NULL, param, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosProcessFilterExecute(NULL, NULL, &input);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosProcessFilterExecute(NULL, NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloProcessFilterExecuteErrParamTeardown(UNUSED void **state) {
    return 0;
}
