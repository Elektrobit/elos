// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdScannerPath_utest.h"

int elosTestElosConfigGetElosdScannerPathSuccessFromEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdScannerPathSuccessFromEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdScannerPathSuccessFromEnv(UNUSED void **state) {
    const char *mockKey = "ELOS_SCANNER_PATH";
    const char *expectedValue = "/usr/local/lib/elos/scanner";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdScannerPath");
    SHOULD("%s", "get the elos scanner path environment option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_SCANNER "Path");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, mockKey);
    expect_value(__wrap_safuGetEnvOr, defaultValue, expectedValue);
    will_return(__wrap_safuGetEnvOr, expectedValue);

    const char *returnValue = elosConfigGetElosdScannerPath(&mockConfig);
    assert_string_equal(returnValue, expectedValue);
}
