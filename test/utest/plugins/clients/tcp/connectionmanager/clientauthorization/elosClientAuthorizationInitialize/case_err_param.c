// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libmnl.h>
#include <tcp_clientauthorization/clientauthorization.h>

#include "elosClientAuthorizationInitialize_utest.h"

int elosTestElosClientAuthorizationInitializeErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientAuthorizationInitializeErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientAuthorizationInitializeErrParam(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosTcpClientAuthorizationInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if parameter is NULL");

    result = elosTcpClientAuthorizationInitialize(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
