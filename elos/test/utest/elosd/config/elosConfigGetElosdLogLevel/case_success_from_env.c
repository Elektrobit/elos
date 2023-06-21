// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <safu/log.h>

#include "cmocka_mocks/mock_libc.h"
#include "elosConfigGetElosdLogLevel_utest.h"

int elosTestElosConfigGetElosdLogLevelSuccessFromEnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdLogLevelSuccessFromEnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdLogLevelSuccessFromEnv(UNUSED void **state) {
    const char *mockKey = "ELOS_LOG_LEVEL";
    const char *expectedString = "Error";
    safuLogLevelE_t expectedValue = SAFU_LOG_LEVEL_ERR;
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdLogLevel");
    SHOULD("%s", "get the elos LogLevel environment option");

    MOCK_FUNC_ENABLE(samconfConfigGetBool);
    expect_value(__wrap_samconfConfigGetBool, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetBool, path, ELOS_CONFIG_ROOT "UseEnv");
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_ENABLE(getenv);
    expect_string(__wrap_getenv, name, mockKey);
    will_return(__wrap_getenv, expectedString);

    safuLogLevelE_t returnValue = elosConfigGetElosdLogLevel(&mockConfig);
    assert_int_equal(returnValue, expectedValue);
}
