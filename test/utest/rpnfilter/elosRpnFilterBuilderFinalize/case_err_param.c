// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderFinalize_utest.h"

int elos_testElosRpnFilterBuilderFinalizeErrParamSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterBuilderFinalizeErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilterBuilder_t data = {0};
    elosRpnFilter_t filter = {0};

    TEST("elosRpnFilterBuilderFinalize");
    SHOULD("%s", "Properly return an error when given wrong parameters");

    result = elosRpnFilterBuilderFinalize(NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterBuilderFinalize(&data, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterBuilderFinalize(NULL, &filter);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elos_testElosRpnFilterBuilderFinalizeErrParamTeardown(UNUSED void **state) {
    return 0;
}
