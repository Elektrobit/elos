// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipSuccessSetup(void **state) {
    elosSession_t *session = elosConnectTcpipCreateSession();
    *state = session;
    return 0;
}

int elosTestElosConnectTcpipSuccessTeardown(void **state) {
    elosSession_t *session = *state;
    free(session);
    return 0;
}

void elosTestElosConnectTcpipSuccess(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "create successfully a tcp-ip connection");

    MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
    expect_string(elosConnectSessionTcpip, host, MOCK_IP_ADDR);
    expect_value(elosConnectSessionTcpip, port, MOCK_PORT);
    expect_any(elosConnectSessionTcpip, session);
    will_return(elosConnectSessionTcpip, SAFU_RESULT_OK);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_OK);

    free(newSession);
}
