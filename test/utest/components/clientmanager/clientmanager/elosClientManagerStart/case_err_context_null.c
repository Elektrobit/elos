// SPDX-License-Identifier: MIT

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartErrContextNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStartErrContextNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStartErrContextNull(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when the context parameter is null");

    retval = elosClientManagerStart(NULL, testState->parameters);

    assert_int_equal(retval, -1);
}
