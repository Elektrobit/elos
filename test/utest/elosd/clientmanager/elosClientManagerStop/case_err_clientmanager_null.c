// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"

int elosTestElosClientManagerStopErrClientManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStopErrClientManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStopErrClientManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerStop");
    SHOULD("not stop clientManager since clientmanager input is null");

    result = elosClientManagerStop(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
