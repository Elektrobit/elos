// SPDX-License-Identifier: MIT

#include <connectionmanager/clientblacklist.h>
#include <safu/result.h>
#include <samconf/test_utils.h>

#include "elosBlacklistInitialize_utest.h"
#include "safu/log.h"
#include "safu/mock_log.h"

int elosTestElosBlacklistInitializeExtErrFilterStringSetup(void **state) {
    elosTestState_t *testState = *state;

    memset(testState, 0, sizeof(elosTestState_t));
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\":{"
        "    \"EventBlacklist\":\"\""
        "  }"
        "}",
        true, &testState->mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosBlacklistInitializeExtErrFilterStringTeardown(void **state) {
    elosTestState_t *testState = *state;
    elosBlacklistDelete(&testState->testFilter);
    samconfConfigDeleteMembers(&testState->mockConfig);
    return 0;
}

void elosTestElosBlacklistInitializeExtErrFilterString(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since filter string is empty");

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "blacklist filter string is not set in config");
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    safuResultE_t result = elosBlacklistInitialize(&testState->testFilter, &testState->mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_not_equal(testState->testFilter.state, RPNFILTER_FINALIZED);
}
