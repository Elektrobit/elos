// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdConnectionLimit_utest.h"

static char *elosCurrentEnvValue = NULL;
int elosTestElosConfigGetElosdConnectionLimitErrorLimitNotIntInEnvSetup(UNUSED void **state) {
    char *envValue = getenv("ELOSD_CONNECTION_LIMIT");
    if (envValue != NULL) {
        elosCurrentEnvValue = strdup(envValue);
    }
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitErrorLimitNotIntInEnvTeardown(UNUSED void **state) {
    if (elosCurrentEnvValue != NULL) {
        setenv("ELOSD_CONNECTION_LIMIT", elosCurrentEnvValue, 1);
        free(elosCurrentEnvValue);
    }
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitErrorLimitNotIntInEnv(UNUSED void **state) {
    TEST("elosTcpConfigGetConnectionLimit");
    SHOULD("%s", "should be 200 since limit environment option is not integer");

    samconfConfig_t mockConfig = {0};
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"ConnectionLimit\": 200"
        "  }"
        "}",
        true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    setenv("ELOSD_CONNECTION_LIMIT", "limit is not an integer", 1);  // environment is restored by setup/teardown
    elosPlugin_t plugin = {.useEnv = true, .config = &mockConfig};

    const int32_t expectedValue = 200;
    int32_t limit = elosTcpConfigGetConnectionLimit(&plugin);
    assert_int_equal(expectedValue, limit);

    samconfConfigDeleteMembers(&mockConfig);
}
