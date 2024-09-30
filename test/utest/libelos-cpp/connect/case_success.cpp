// SPDX-License-Identifier: MIT
#include "connect_utest.h"

int elosTestConnectSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestConnectSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestConnectSuccess(UNUSED void **state) {
    using namespace elos;
    elosSession_t *testSession = NULL;
    safuResultE_t result;
    Elos testObject;

    TEST("connect");
    SHOULD("%s", "create successfully a tcp-ip connection");

    result = testObject.connect("0.0.0.0", 54321, &testSession);
    assert_int_equal(result, SAFU_RESULT_OK);
}
