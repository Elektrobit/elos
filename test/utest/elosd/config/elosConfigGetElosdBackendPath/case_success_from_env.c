// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdBackendPath_utest.h"

int elosTestElosConfigGetElosdBackendPathSuccessFromEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdBackendPathSuccessFromEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdBackendPathSuccessFromEnv(UNUSED void **state) {
    const char *mockKey = "ELOS_BACKEND_PATH";
    const char *expectedValue = "/usr/local/lib/elos/backend";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdBackendPath");
    SHOULD("%s", "get the elos backend path environment option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_ROOT "EventLogging/PluginSearchPath");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, mockKey);
    expect_value(__wrap_safuGetEnvOr, defaultValue, expectedValue);
    will_return(__wrap_safuGetEnvOr, expectedValue);

    const char *returnValue = elosConfigGetElosdBackendPath(&mockConfig);
    assert_string_equal(returnValue, expectedValue);
}
