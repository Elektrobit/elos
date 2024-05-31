// SPDX-License-Identifier: MIT

#include <connectionmanager/clientblacklist.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <samconf/test_utils.h>

#include "elosBlacklistInitialize_utest.h"

int elosTestElosBlacklistInitializeSuccessSetup(void **state) {
    elosTestState_t *testState = *state;

    memset(testState, 0, sizeof(elosTestState_t));
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\":{"
        "    \"EventBlacklist\":\".event.messageCode 2000 EQ\""
        "  }"
        "}",
        true, &testState->mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosBlacklistInitializeSuccessTeardown(void **state) {
    elosTestState_t *testState = *state;
    elosBlacklistDelete(&testState->testFilter);
    samconfConfigDeleteMembers(&testState->mockConfig);
    return 0;
}

void elosTestElosBlacklistInitializeSuccess(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "initialize connection blacklist successfully");

    safuResultE_t result = elosBlacklistInitialize(&testState->testFilter, &testState->mockConfig);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testState->testFilter.state, RPNFILTER_FINALIZED);
}
