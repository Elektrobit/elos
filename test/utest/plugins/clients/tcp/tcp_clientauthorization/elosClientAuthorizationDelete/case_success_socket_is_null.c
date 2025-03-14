// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationDelete_utest.h"

int elosTestElosClientAuthorizationDeleteSuccessSocketIsNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationDeleteSuccessSocketIsNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationDeleteSuccessSocketIsNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosTcpClientAuthorizationDelete");
    SHOULD("%s", "tread clientAuth->mnl_socket set to NULL as already closed and report success");

    elosClientAuthorization_t clientAuth = {.socketData = NULL};

    result = elosTcpClientAuthorizationDelete(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(clientAuth.socketData);
}
