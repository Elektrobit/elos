// SPDX-License-Identifier: MIT

#include <sys/types.h>

#include "elosAuthorizedProcessInitialize_utest.h"

int elosTestElosAuthorizedProcessInitializeConfigParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessInitializeConfigParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosAuthorizedProcessInitializeConfigParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t testFilter = {0};

    TEST("elosAuthorizedProcessInitialize");
    SHOULD("%s", "authorized process filter not initialized since config parameter is null");

    result = elosAuthorizedProcessInitialize(&testFilter, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
