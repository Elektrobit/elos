// SPDX-License-Identifier: MIT

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartErrClientManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStartErrClientManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStartErrClientManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerStart");
    SHOULD("not start clientManager since clientmanager input is null");

    result = elosClientManagerStart(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
