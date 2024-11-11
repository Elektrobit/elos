// SPDX-License-Identifier: MIT
#include "disconnect_utest.h"

int elosTestDisconnectSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestDisconnectSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestDisconnectSuccess(UNUSED void **state) {
    using namespace elos;
    elosResultE result;
    Elos testObject(MOCK_IP_ADDR, MOCK_PORT);

    TEST("disconnect");
    SHOULD("%s", "successfully close a connection");

    MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
    expect_string(elosConnectSessionTcpip, host, MOCK_IP_ADDR);
    expect_value(elosConnectSessionTcpip, port, MOCK_PORT);
    expect_any(elosConnectSessionTcpip, session);
    will_return(elosConnectSessionTcpip, ELOS_RESULT_OK);

    testObject.connect();

    MOCK_FUNC_AFTER_CALL(elosDisconnectSession, 0);
    expect_any(elosDisconnectSession, session);
    will_return(elosDisconnectSession, ELOS_RESULT_OK);

    result = testObject.disconnect();
    assert_int_equal(result, ELOS_RESULT_OK);
}
