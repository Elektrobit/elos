// SPDX-License-Identifier: MIT

#include "elosBlacklistInitialize_utest.h"

int elosTestElosBlacklistInitializeErrBlacklistParameterNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeErrBlacklistParameterNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeErrBlacklistParameterNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since null blacklist parameter");

    result = elosBlacklistInitialize(NULL, &mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
