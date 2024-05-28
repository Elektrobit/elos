// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitSuccessFromDefault(UNUSED void **state) {
    int32_t limit = 0;
    int32_t expectedValue = 200;
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosTcpConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit default option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, false);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigGetInt32, 0);
    expect_value(__wrap_samconfConfigGetInt32, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetInt32, path, ELOS_CONFIG_ROOT "ConnectionLimit");
    expect_any(__wrap_samconfConfigGetInt32, result);
    will_set_parameter(__wrap_samconfConfigGetInt32, result, expectedValue);
    will_return(__wrap_samconfConfigGetInt32, SAMCONF_CONFIG_OK);

    limit = elosTcpConfigGetConnectionLimit(&mockConfig);

    assert_int_equal(expectedValue, limit);
}
