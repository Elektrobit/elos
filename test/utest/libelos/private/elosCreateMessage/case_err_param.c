// SPDX-License-Identifier: MIT

#include "elosCreateMessage_utest.h"

int elosTestElosCreateMessageErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosCreateMessageErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosCreateMessageErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosCreateMessage");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    // Note: First and second parameters are allowed to be 0/NULL and do not need to be tested here

    PARAM("%s", "0, NULL, NULL");
    result = elosCreateMessage(0, NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
