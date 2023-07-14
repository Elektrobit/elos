// SPDX-License-Identifier: MIT

#include "elosEventFilterBuilderCompile_utest.h"

int elosTestEloEventFilterBuilderCompileErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterBuilderCompileErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilterBuilder_t data = {0};

    TEST("elosEventFilterBuilderCompile");
    SHOULD("%s", "Properly return an error when given wrong parameters");

    result = elosEventFilterBuilderCompile(NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    result = elosEventFilterBuilderCompile(&data);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloEventFilterBuilderCompileErrParamTeardown(UNUSED void **state) {
    return 0;
}
