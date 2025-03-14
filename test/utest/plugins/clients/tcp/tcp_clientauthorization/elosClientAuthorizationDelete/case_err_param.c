// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationDelete_utest.h"

int elosTestElosClientAuthorizationDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationDeleteErrParam(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosTcpClientAuthorizationDelete");
    SHOULD("%s", "return SAFU_RESULT_FAILED and let clientAuth.mlSocket unchanged if parameter is NULL");

    result = elosTcpClientAuthorizationDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
