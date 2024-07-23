// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdPort_utest.h"

static char *elosCurrentEnvValue = NULL;

int elosTestElosConfigGetElosdPortSuccessFromEnvSetup(UNUSED void **state) {
    char *envValue = getenv("ELOSD_PORT");
    if (envValue != NULL) {
        elosCurrentEnvValue = strdup(envValue);
    }
    return 0;
}

int elosTestElosConfigGetElosdPortSuccessFromEnvTeardown(UNUSED void **state) {
    if (elosCurrentEnvValue != NULL) {
        setenv("ELOSD_PORT", elosCurrentEnvValue, 1);
        free(elosCurrentEnvValue);
    }
    return 0;
}

void elosTestElosConfigGetElosdPortSuccessFromEnv(UNUSED void **state) {
    TEST("elosTcpConfigGetPort");
    SHOULD("%s", "get the elos port environment option");

    samconfConfig_t mockConfig = {0};
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"port\": 54321"
        "  }"
        "}",
        true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    setenv("ELOSD_PORT", "42", 1);  // environment is restored by setup/teardown
    elosPlugin_t plugin = {.useEnv = true, .config = &mockConfig};

    int port = elosTcpConfigGetPort(&plugin);
    assert_int_equal(42, port);

    samconfConfigDeleteMembers(&mockConfig);
}
