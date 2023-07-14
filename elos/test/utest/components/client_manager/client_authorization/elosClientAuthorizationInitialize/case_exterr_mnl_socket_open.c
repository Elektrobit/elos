// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libmnl.h>
#include <elos/client_manager/client_authorization.h>

#include "elosClientAuthorizationInitialize_utest.h"

int elosTestElosClientAuthorizationInitializeExterrMnlSocketOpenSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationInitializeExterrMnlSocketOpenTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationInitializeExterrMnlSocketOpen(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED and let clientAuth.mlSock unchanged if mnl_socket_open failed");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.mlSocket = expectedNlSocket};

    MOCK_FUNC_AFTER_CALL(mnl_socket_open, 0);
    expect_value(__wrap_mnl_socket_open, bus, NETLINK_INET_DIAG);
    will_return(__wrap_mnl_socket_open, NULL);

    result = elosClientAuthorizationInitialize(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_ptr_equal(clientAuth.mlSocket, expectedNlSocket);
}
