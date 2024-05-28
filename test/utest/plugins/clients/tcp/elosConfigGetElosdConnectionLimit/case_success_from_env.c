// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitSuccessFromEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitSuccessFromEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitSuccessFromEnv(UNUSED void **state) {
    int32_t limit = 0;
    int32_t expectedValue = 100;
    char limitAsString[] = "100";
    const char *mockKey = "ELOSD_CONNECTION_LIMIT";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosTcpConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit environment option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, mockKey);
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, limitAsString);

    limit = elosTcpConfigGetConnectionLimit(&mockConfig);

    assert_int_equal(expectedValue, limit);
}
