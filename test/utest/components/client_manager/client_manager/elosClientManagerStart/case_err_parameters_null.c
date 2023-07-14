// SPDX-License-Identifier: MIT

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartErrParametersNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStartErrParametersNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStartErrParametersNull(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when 'parameters' is null");

    retval = elosClientManagerStart(testState->context, NULL);

    assert_int_equal(retval, -1);
}
