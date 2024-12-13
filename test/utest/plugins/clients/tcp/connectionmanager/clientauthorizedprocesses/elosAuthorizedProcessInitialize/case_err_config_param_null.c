// SPDX-License-Identifier: MIT
#include <clientauthorizedprocesses/clientauthorizedprocesses.h>
#include <safu/result.h>
#include <samconf/test_utils.h>
#include <string.h>

#include "elosAuthorizedProcessInitialize_utest.h"

int elosTestElosAuthorizedProcessInitializeConfigParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessInitializeConfigParamNullTeardown(void **state) {
    elosTestState_t *testState = *state;

    elosAuthorizedProcessDelete(&testState->testFilter);

    return 0;
}

void elosTestElosAuthorizedProcessInitializeConfigParamNull(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosAuthorizedProcessInitialize");
    SHOULD("%s", "authorized process filter not initialized since config parameter is null");

    safuResultE_t result = elosAuthorizedProcessInitialize(&testState->testFilter, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
