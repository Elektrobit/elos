// SPDX-License-Identifier: MIT

#include "elosProcessFilterBuilderCompile_utest.h"

int elosTestEloProcessFilterBuilderCompileErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloProcessFilterBuilderCompileErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilterBuilder_t data = {0};

    TEST("elosProcessFilterBuilderCompile");
    SHOULD("%s", "Properly return an error when given wrong parameters");

    result = elosProcessFilterBuilderCompile(NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosProcessFilterBuilderCompile(&data);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloProcessFilterBuilderCompileErrParamTeardown(UNUSED void **state) {
    return 0;
}
