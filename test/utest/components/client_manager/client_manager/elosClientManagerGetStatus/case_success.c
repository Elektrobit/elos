// SPDX-License-Identifier: MIT

#include "elosClientManagerGetStatus_utest.h"

int elosTestElosClientManagerGetStatusSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerGetStatusSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerGetStatusSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientManagerContext_t *context = *state;
    uint32_t flags = 0;

    TEST("elosClientManagerGetStatus");
    SHOULD("%s", "successfully return the flags value");

    result = elosClientManagerGetStatus(context, &flags);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(flags, MOCK_FLAGS);
}
