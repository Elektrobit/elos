// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosUnixConfigGetElosdConnectionLimit_utest.h"

static char *elosCurrentEnvValue = NULL;

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromEnvSetup(UNUSED void **state) {
    char *envValue = getenv("ELOSD_CONNECTION_LIMIT");
    if (envValue != NULL) {
        elosCurrentEnvValue = strdup(envValue);
    }
    return 0;
}

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromEnvTeardown(UNUSED void **state) {
    if (elosCurrentEnvValue != NULL) {
        setenv("ELOSD_CONNECTION_LIMIT", elosCurrentEnvValue, 1);
        free(elosCurrentEnvValue);
    }
    return 0;
}

void elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromEnv(UNUSED void **state) {
    TEST("elosUnixConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit environment option");

    samconfConfig_t mockConfig = {0};
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"ConnectionLimit\": 42"
        "  }"
        "}",
        true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    setenv("ELOSD_CONNECTION_LIMIT", "100", 1);  // environment is restored by setup/teardown
    elosPlugin_t plugin = {.useEnv = true, .config = &mockConfig};

    const int32_t expectedValue = 100;
    int32_t limit = elosUnixConfigGetConnectionLimit(&plugin);
    samconfConfigDeleteMembers(&mockConfig);
    assert_int_equal(expectedValue, limit);
}
