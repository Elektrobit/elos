// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInDefault(UNUSED void **state) {
    int32_t limit = 0;
    int32_t expectedValue = 200;
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdConnectionLimit");
    SHOULD("%s", "be 200 since default is invalid");

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
    will_set_parameter(__wrap_samconfConfigGetInt32, result, 0);
    will_return(__wrap_samconfConfigGetInt32, SAMCONF_CONFIG_INVALID_TYPE);

    limit = elosConfigGetElosdConnectionLimit(&mockConfig);

    assert_int_equal(expectedValue, limit);
}
