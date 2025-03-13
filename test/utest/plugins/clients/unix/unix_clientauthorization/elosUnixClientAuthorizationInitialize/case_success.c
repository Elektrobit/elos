// SPDX-License-Identifier: MIT

#include <unix_clientauthorization/clientauthorization.h>

#include "elosUnixClientAuthorizationInitialize_utest.h"

int elosTestelosUnixClientAuthorizationInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixClientAuthorizationInitializeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixClientAuthorizationInitializeSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosUnixClientAuthorizationInitialize");
    SHOULD("%s", "successfully initialize");

    elosClientAuthorization_t clientAuth = {0};

    result = elosUnixClientAuthorizationInitialize(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
}
