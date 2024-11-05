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
    elosResultE result;
    Elos testObject(MOCK_IP_ADDR, MOCK_PORT);

    TEST("connect");
    SHOULD("%s", "create successfully a tcp-ip connection");

    result = testObject.connect();
    assert_int_equal(result, ELOS_RESULT_OK);
}
