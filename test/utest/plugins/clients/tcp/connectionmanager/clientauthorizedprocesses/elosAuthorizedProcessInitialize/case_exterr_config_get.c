// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/clientauthorizedprocesses.h>
#include <safu/result.h>
#include <samconf/test_utils.h>
#include <string.h>

#include "elosAuthorizedProcessInitialize_utest.h"

int elosTestElosAuthorizedProcessInitializeExtErrConfigGetSetup(void **state) {
    elosTestState_t *testState = *state;

    memset(testState, 0, sizeof(elosTestState_t));
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\":{"
        "  }"
        "}",
        true, &testState->mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosAuthorizedProcessInitializeExtErrConfigGetTeardown(void **state) {
    elosTestState_t *testState = *state;

    elosAuthorizedProcessDelete(&testState->testFilter);
    samconfConfigDeleteMembers(&testState->mockConfig);

    return 0;
}

void elosTestElosAuthorizedProcessInitializeExtErrConfigGet(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosAuthorizedProcessInitialize");
    SHOULD("%s", "authorized process filter not initialized since config attribute not found");

    safuResultE_t result = elosAuthorizedProcessInitialize(&testState->testFilter, &testState->mockConfig);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
