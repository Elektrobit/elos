// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdInterface_utest.h"

static char *elosCurrentEnvValue = NULL;
int elosTestElosConfigGetElosdInterfaceSuccessFromEnvSetup(UNUSED void **state) {
    char *envValue = getenv("ELOSD_INTERFACE");
    if (envValue != NULL) {
        elosCurrentEnvValue = strdup(envValue);
    }
    return 0;
}

int elosTestElosConfigGetElosdInterfaceSuccessFromEnvTeardown(UNUSED void **state) {
    if (elosCurrentEnvValue != NULL) {
        setenv("ELOSD_INTERFACE", elosCurrentEnvValue, 1);
        free(elosCurrentEnvValue);
    }
    return 0;
}

void elosTestElosConfigGetElosdInterfaceSuccessFromEnv(UNUSED void **state) {
    TEST("elosTcpConfigGetInterface");
    SHOULD("%s", "get the elos interface environment option");

    const char *expectedInterface = "127.0.0.1";
    samconfConfig_t mockConfig = {0};
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"Interface\": \"192.4.2.1\""
        "  }"
        "}",
        true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    setenv("ELOSD_INTERFACE", expectedInterface, 1);  // environment is restored by setup/teardown
    elosPlugin_t plugin = {.useEnv = true, .config = &mockConfig};

    const char *returnValue = elosTcpConfigGetInterface(&plugin);
    assert_string_equal(returnValue, expectedInterface);

    samconfConfigDeleteMembers(&mockConfig);
}
