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

    MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
    expect_string(elosConnectSessionTcpip, host, MOCK_IP_ADDR);
    expect_value(elosConnectSessionTcpip, port, MOCK_PORT);
    expect_any(elosConnectSessionTcpip, session);
    will_return(elosConnectSessionTcpip, ELOS_RESULT_OK);

    result = testObject.connect();
    assert_int_equal(result, ELOS_RESULT_OK);
}
