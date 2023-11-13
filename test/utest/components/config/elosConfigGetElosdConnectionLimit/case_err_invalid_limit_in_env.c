// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInEnv(UNUSED void **state) {
    int32_t limit = 0;
    int32_t expectedValue = 200;
    const char *mockKey = "ELOSD_CONNECTION_LIMIT";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdConnectionLimit");
    SHOULD("%s", "should be 200 since limit environment option is invalid");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, mockKey);
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, NULL);

    limit = elosConfigGetElosdConnectionLimit(&mockConfig);

    assert_int_equal(expectedValue, limit);
}
