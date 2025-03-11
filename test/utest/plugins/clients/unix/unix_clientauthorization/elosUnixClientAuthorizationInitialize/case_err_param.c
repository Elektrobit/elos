// SPDX-License-Identifier: MIT

#include <unix_clientauthorization/clientauthorization.h>

#include "elosUnixClientAuthorizationInitialize_utest.h"

int elosTestelosUnixClientAuthorizationInitializeErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixClientAuthorizationInitializeErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixClientAuthorizationInitializeErrParam(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosUnixClientAuthorizationInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if parameter is NULL");

    result = elosUnixClientAuthorizationInitialize(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
