// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationDelete_utest.h"

int elosTestElosClientAuthorizationDeleteExterrMnlSocketCloseSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationDeleteExterrMnlSocketCloseTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationDeleteExterrMnlSocketClose(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosTcpClientAuthorizationDelete");
    SHOULD("%s", "return SAFU_RESULT_FAILED and let clientAuth.mlSock unchanged if mnl_socket_close failed");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.socketData = (void *)expectedNlSocket};

    MOCK_FUNC_AFTER_CALL(mnl_socket_close, 0);
    expect_value(__wrap_mnl_socket_close, nl, expectedNlSocket);
    will_return(__wrap_mnl_socket_close, LIBMNL_ERROR);

    result = elosTcpClientAuthorizationDelete(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_ptr_equal(clientAuth.socketData, expectedNlSocket);
}
