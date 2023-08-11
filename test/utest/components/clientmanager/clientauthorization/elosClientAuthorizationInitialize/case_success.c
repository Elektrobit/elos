// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libmnl.h>
#include <elos/clientmanager/clientauthorization.h>

#include "elosClientAuthorizationInitialize_utest.h"

int elosTestElosClientAuthorizationInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationInitializeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationInitializeSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationInitialize");
    SHOULD("%s", "successfully initialize a netlink inet_diag connection");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.mlSocket = NULL};

    MOCK_FUNC_AFTER_CALL(mnl_socket_open, 0);
    expect_value(__wrap_mnl_socket_open, bus, NETLINK_INET_DIAG);
    will_return(__wrap_mnl_socket_open, expectedNlSocket);

    MOCK_FUNC_AFTER_CALL(mnl_socket_bind, 0);
    expect_value(__wrap_mnl_socket_bind, nl, expectedNlSocket);
    expect_value(__wrap_mnl_socket_bind, groups, 0);
    expect_value(__wrap_mnl_socket_bind, pid, MNL_SOCKET_AUTOPID);
    will_return(__wrap_mnl_socket_bind, LIBMNL_OK);

    result = elosClientAuthorizationInitialize(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(clientAuth.mlSocket, expectedNlSocket);
}
