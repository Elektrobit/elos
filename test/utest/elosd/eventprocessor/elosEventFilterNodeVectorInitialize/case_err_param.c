// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorInitialize_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorInitializeErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeVectorInitializeErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeVectorInitializeErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    result = elosEventFilterNodeVectorInitialize(NULL, _VECTORELEMENTS);
    assert_true(result == SAFU_RESULT_FAILED);
}
