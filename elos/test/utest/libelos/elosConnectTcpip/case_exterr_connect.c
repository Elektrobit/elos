// SPDX-License-Identifier: MIT

#include <arpa/inet.h>

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipExtErrConnectSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipExtErrConnectTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipExtErrConnect(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling connect fails");

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
    will_return(__wrap_connect, -1);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
