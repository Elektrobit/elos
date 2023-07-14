// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdScannerPath_utest.h"

int elosTestElosConfigGetElosdScannerPathSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdScannerPathSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdScannerPathSuccessFromDefault(UNUSED void **state) {
    const char *expectedValue = "/usr/local/lib/elos/scanner";
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdScannerPath");
    SHOULD("%s", "get the elos scanner path default option");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, false);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_SCANNER "Path");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    const char *returnValue = elosConfigGetElosdScannerPath(&mockConfig);
    assert_string_equal(returnValue, expectedValue);
}
