// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationInitialize_utest.h"

int elosTestElosClientAuthorizationInitializeExterrMnlSocketBindSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationInitializeExterrMnlSocketBindTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationInitializeExterrMnlSocketBind(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED and let clientAuth.mlSock unchanged if mnl_socket_bind failed");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.socketData = NULL};

    MOCK_FUNC_AFTER_CALL(mnl_socket_open, 0);
    expect_value(__wrap_mnl_socket_open, bus, NETLINK_INET_DIAG);
    will_return(__wrap_mnl_socket_open, expectedNlSocket);

    MOCK_FUNC_AFTER_CALL(mnl_socket_bind, 0);
    expect_value(__wrap_mnl_socket_bind, nl, expectedNlSocket);
    expect_value(__wrap_mnl_socket_bind, groups, 0);
    expect_value(__wrap_mnl_socket_bind, pid, MNL_SOCKET_AUTOPID);
    will_return(__wrap_mnl_socket_bind, LIBMNL_ERROR);

    result = elosTcpClientAuthorizationInitialize(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_ptr_equal(clientAuth.socketData, NULL);
}
