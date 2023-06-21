// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"

int elosTestElosClientManagerStopErrContextNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStopErrContextNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStopErrContextNull(UNUSED void **state) {
    int retval = 0;

    TEST("elosClientManagerStop");
    SHOULD("%s", "return -1 when the context parameter is null");

    retval = elosClientManagerStop(NULL);

    assert_int_equal(retval, -1);
}
