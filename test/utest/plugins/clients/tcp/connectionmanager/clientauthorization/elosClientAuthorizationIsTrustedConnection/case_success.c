// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationIsTrustedConnection_utest.h"

int elosTestElosClientAuthorizationIsTrustedConnectionSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationIsTrustedConnectionSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationIsTrustedConnectionSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationIsTrustedConnection");
    SHOULD("%s", "successfully do a client authorization lookup");

    SKIP(
        "This test shall check at least a success full client lookup, but this is not that trivial we need some "
        "refactoring to get useful tests. See for details #18047");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.mlSocket = expectedNlSocket};
    struct sockaddr addr = {0};

    MOCK_FUNC_AFTER_CALL(mnl_socket_sendto, 0);
    expect_value(__wrap_mnl_socket_sendto, nl, expectedNlSocket);
    expect_any(__wrap_mnl_socket_sendto, req);
    expect_any(__wrap_mnl_socket_sendto, siz);
    will_return(__wrap_mnl_socket_sendto, LIBMNL_OK);

    MOCK_FUNC_AFTER_CALL(mnl_socket_get_portid, 0);
    expect_value(__wrap_mnl_socket_get_portid, nl, expectedNlSocket);
    will_return(__wrap_mnl_socket_get_portid, 1234);

    MOCK_FUNC_AFTER_CALL(mnl_socket_recvfrom, 0);
    expect_value(__wrap_mnl_socket_recvfrom, nl, expectedNlSocket);
    expect_any(__wrap_mnl_socket_recvfrom, buf);
    expect_any(__wrap_mnl_socket_recvfrom, siz);
    will_return(__wrap_mnl_socket_recvfrom, 0);

    result = elosTcpClientAuthorizationIsTrustedConnection(&clientAuth, &addr);
    assert_true(result);
}
