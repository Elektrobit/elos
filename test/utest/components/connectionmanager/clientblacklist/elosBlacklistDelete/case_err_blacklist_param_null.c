// SPDX-License-Identifier: MIT

#include "elosBlacklistDelete_utest.h"

int elosTestElosBlacklistDeleteErrBlacklistParameterNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistDeleteErrBlacklistParameterNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistDeleteErrBlacklistParameterNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosBlacklistDelete");
    SHOULD("%s", "not delete blacklist filter since null blacklist parameter");

    result = elosBlacklistDelete(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
