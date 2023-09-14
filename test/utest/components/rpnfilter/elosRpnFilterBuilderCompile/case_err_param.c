// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"

int elos_testElosRpnFilterBuilderCompileErrParamSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterBuilderCompileErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilterBuilder_t data = {0};
    elosRpnFilterBuilderCompileFunc_t func[] = {NULL};

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly return an error when given wrong parameters");

    result = elosRpnFilterBuilderCompileWith(NULL, NULL, 0);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterBuilderCompileWith(&data, NULL, 0);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterBuilderCompileWith(&data, NULL, 1);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosRpnFilterBuilderCompileWith(&data, func, 0);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elos_testElosRpnFilterBuilderCompileErrParamTeardown(UNUSED void **state) {
    return 0;
}
