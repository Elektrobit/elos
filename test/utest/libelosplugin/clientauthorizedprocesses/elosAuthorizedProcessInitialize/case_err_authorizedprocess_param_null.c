// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/clientauthorizedprocesses.h>

#include "elosAuthorizedProcessInitialize_utest.h"

int elosTestElosAuthorizedProcessInitializeAuthorizedprocessParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessInitializeAuthorizedprocessParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosAuthorizedProcessInitializeAuthorizedprocessParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfig_t mockConfig = {0};

    TEST("elosAuthorizedProcessInitialize");
    SHOULD("%s", "authorized process filter not initialized since filter parameter is null");

    result = elosAuthorizedProcessInitialize(NULL, &mockConfig);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
