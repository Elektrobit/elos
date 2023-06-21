// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdPort_utest.h"

int elosTestElosConfigGetElosdPortSuccessFromEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdPortSuccessFromEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdPortSuccessFromEnv(UNUSED void **state) {
    int32_t port = 0;
    int32_t expectedValue = 54321;
    char portAsString[] = "54321";
    const char *mockKey = "ELOSD_PORT";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdPort");
    SHOULD("%s", "get the elos port environment option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, mockKey);
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, portAsString);

    port = elosConfigGetElosdPort(&mockConfig);

    assert_int_equal(expectedValue, port);
}
