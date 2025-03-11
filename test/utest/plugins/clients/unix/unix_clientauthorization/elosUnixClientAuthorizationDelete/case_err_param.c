// SPDX-License-Identifier: MIT

#include <unix_clientauthorization/clientauthorization.h>

#include "elosUnixClientAuthorizationDelete_utest.h"

int elosTestelosUnixClientAuthorizationDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixClientAuthorizationDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixClientAuthorizationDeleteErrParam(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosUnixClientAuthorizationDelete");
    SHOULD("%s", "return SAFU_RESULT_FAILED");

    result = elosUnixClientAuthorizationDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
