// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libmnl.h>
#include <connectionmanager/clientauthorization.h>

#include "elosClientAuthorizationDelete_utest.h"

int elosTestElosClientAuthorizationDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationDeleteSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationDelete");
    SHOULD("%s", "successfully close a netlink inet_diag connection");

    struct mnl_socket *expectedNlSocket = (struct mnl_socket *)0xDEADBEEF;
    elosClientAuthorization_t clientAuth = {.mlSocket = expectedNlSocket};

    MOCK_FUNC_AFTER_CALL(mnl_socket_close, 0);
    expect_value(__wrap_mnl_socket_close, nl, expectedNlSocket);
    will_return(__wrap_mnl_socket_close, LIBMNL_OK);

    result = elosClientAuthorizationDelete(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(clientAuth.mlSocket);
}
