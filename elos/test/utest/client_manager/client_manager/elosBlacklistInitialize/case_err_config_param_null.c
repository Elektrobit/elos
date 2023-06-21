// SPDX-License-Identifier: MIT

#include "elosBlacklistInitialize_utest.h"

int elosTestElosBlacklistInitializeErrConfigParameterNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeErrConfigParameterNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeErrConfigParameterNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter;

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since null config parameter");

    result = elosBlacklistInitialize(&testFilter, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
