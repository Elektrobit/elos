// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipExtErrSocketSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipExtErrSocketTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipExtErrSocket(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;
    struct addrinfo infoRes = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
    };

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling socket fails");

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
    will_return(__wrap_socket, -1);

    MOCK_FUNC_AFTER_CALL(freeaddrinfo, 0);
    expect_value(__wrap_freeaddrinfo, res, &infoRes);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
