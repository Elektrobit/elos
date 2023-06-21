// SPDX-License-Identifier: MIT

#include "elosRpnFilterExecute_utest.h"

int elos_testElosRpnFilterExecuteErrParamSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterExecuteErrParam(UNUSED void **state) {
    elosRpnFilter_t filter = {0};
    elosRpnFilterResultE_t result;

    ELOS_FILTERSTACK_NEW(input, 1)

    TEST("elosRpnFilterExecute");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR");

    result = elosRpnFilterExecute(&filter, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterExecute(NULL, input);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterExecute(NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elos_testElosRpnFilterExecuteErrParamTeardown(UNUSED void **state) {
    return 0;
}
