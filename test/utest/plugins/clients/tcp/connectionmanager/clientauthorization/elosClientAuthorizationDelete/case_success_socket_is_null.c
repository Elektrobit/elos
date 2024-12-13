// SPDX-License-Identifier: MIT

#include <tcp_clientauthorization/clientauthorization.h>
#include <cmocka_mocks/mock_libmnl.h>

#include "elosClientAuthorizationDelete_utest.h"

int elosTestElosClientAuthorizationDeleteSuccessSocketIsNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationDeleteSuccessSocketIsNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationDeleteSuccessSocketIsNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientAuthorizationDelete");
    SHOULD("%s", "tread clientAuth->mnl_socket set to NULL as already closed and report success");

    elosClientAuthorization_t clientAuth = {.mlSocket = NULL};

    result = elosClientAuthorizationDelete(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(clientAuth.mlSocket);
}
