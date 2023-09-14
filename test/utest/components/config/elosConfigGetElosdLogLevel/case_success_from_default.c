// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <safu/log.h>

#include "cmocka_mocks/mock_libc.h"
#include "elosConfigGetElosdLogLevel_utest.h"

int elosTestElosConfigGetElosdLogLevelSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdLogLevelSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdLogLevelSuccessFromDefault(UNUSED void **state) {
    safuLogLevelE_t expectedValue = safuLogLevelFromString(ELOSD_LOG_LEVEL);
    samconfConfig_t mockConfig = elosGetMockConfig();
    const char *mockKey = "ELOS_LOG_LEVEL";

    TEST("elosConfigGetElosdLogLevel");
    SHOULD("%s", "get the elos LogLevel default option");

    safuLogLevelE_t returnValue = elosConfigGetElosdLogLevel(&mockConfig);
    assert_int_equal(returnValue, expectedValue);

    MOCK_FUNC_ENABLE(samconfConfigGetBool);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_ENABLE(getenv);
    expect_string(__wrap_getenv, name, mockKey);
    will_return(__wrap_getenv, NULL);

    returnValue = elosConfigGetElosdLogLevel(&mockConfig);
    assert_int_equal(returnValue, expectedValue);
}
