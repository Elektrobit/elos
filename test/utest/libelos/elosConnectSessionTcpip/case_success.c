// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectSessionTcpip_utest.h"

int elosTestElosConnectSessionTcpipSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectSessionTcpipSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectSessionTcpipSuccess(UNUSED void **state) {
    elosSession_t newSession = {MOCK_FD, false};
    safuResultE_t result;
    struct addrinfo infoRes = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
    };

    TEST("elosConnectTcpip");
    SHOULD("%s", "create successfully a tcp-ip connection");

    MOCK_FUNC_AFTER_CALL(getaddrinfo, 0);
    expect_value(__wrap_getaddrinfo, node, MOCK_IP_ADDR);
    expect_any(__wrap_getaddrinfo, service);
    expect_any(__wrap_getaddrinfo, hints);
    will_set_parameter(__wrap_getaddrinfo, res, &infoRes);
    will_return(__wrap_getaddrinfo, 0);

    MOCK_FUNC_AFTER_CALL(socket, 0);
    expect_value(__wrap_socket, domain, AF_INET);
    expect_value(__wrap_socket, type, SOCK_STREAM);
    expect_value(__wrap_socket, protocol, 0);
    will_return(__wrap_socket, MOCK_FD);

    MOCK_FUNC_AFTER_CALL(connect, 0);
    expect_value(__wrap_connect, fd, MOCK_FD);
    expect_any(__wrap_connect, addr);
    expect_any(__wrap_connect, len);
    will_return(__wrap_connect, 0);

    MOCK_FUNC_AFTER_CALL(freeaddrinfo, 0);
    expect_value(__wrap_freeaddrinfo, res, &infoRes);

    result = elosConnectSessionTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(newSession.fd, MOCK_FD);
    assert_int_equal(newSession.connected, true);
    assert_int_equal(result, SAFU_RESULT_OK);
}
