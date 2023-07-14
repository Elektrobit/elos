// SPDX-License-Identifier: MIT
#include <sys/types.h>

#include "elosAuthorizedProcessDelete_utest.h"

int elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosAuthorizedProcessDelete");
    SHOULD("%s", "authorized process filter not deleted since filter parameter is null");

    result = elosAuthorizedProcessDelete(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
