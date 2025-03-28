// SPDX-License-Identifier: MIT
#include <elos/libelosplugin/clientauthorizedprocesses.h>
#include <samconf/test_utils.h>
#include <string.h>
#include <sys/types.h>

#include "elosAuthorizedProcessCheck_utest.h"

int elosTestElosAuthorizedProcessCheckProcessParamNullSetup(void **state) {
    elosTestState_t *testState = *state;

    memset(testState, 0, sizeof(elosTestState_t));
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\":{"
        "    \"authorizedProcesses\": ["
        "      \".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND\","
        "      \".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND\","
        "      \".process.pid 0 EQ\""
        "      ]"
        "  }"
        "}",
        true, &testState->mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    safuResultE_t result = elosAuthorizedProcessInitialize(&testState->testFilter, &testState->mockConfig);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testState->testFilter.elementCount, 3);

    return 0;
}

int elosTestElosAuthorizedProcessCheckProcessParamNullTeardown(void **state) {
    elosTestState_t *testState = *state;

    elosAuthorizedProcessDelete(&testState->testFilter);
    samconfConfigDeleteMembers(&testState->mockConfig);

    return 0;
}

void elosTestElosAuthorizedProcessCheckProcessParamNull(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosAuthorizedProcessCheck");
    SHOULD("%s", "not match process with authorized process filter since process parameter is null");

    bool result = elosAuthorizedProcessCheck(&testState->testFilter, NULL);
    assert_false(result);
}
