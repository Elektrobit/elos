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

void elosTestElosConnectTcpipSuccess(void **state) {
    elosSession_t *expectedSession = *state;
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "create successfully a tcp-ip connection");

    MOCK_FUNC_AFTER_CALL(inet_aton, 0);
    expect_value(__wrap_inet_aton, cp, MOCK_IP_ADDR);
    expect_any(__wrap_inet_aton, inp);
    will_return(__wrap_inet_aton, 0);

    MOCK_FUNC_AFTER_CALL(socket, 0);
    expect_value(__wrap_socket, domain, AF_INET);
    expect_value(__wrap_socket, type, SOCK_STREAM);
    expect_value(__wrap_socket, protocol, 0);
    will_return(__wrap_socket, MOCK_FD);

    MOCK_FUNC_AFTER_CALL(connect, 0);
    expect_value(__wrap_connect, fd, MOCK_FD);
    expect_any(__wrap_connect, addr);
    expect_value(__wrap_connect, len, sizeof(struct sockaddr_in));
    will_return(__wrap_connect, 0);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(newSession->fd, expectedSession->fd);
    assert_int_equal(newSession->addr.sin_family, expectedSession->addr.sin_family);
    assert_int_equal(newSession->addr.sin_port, expectedSession->addr.sin_port);

    free(newSession);
}
