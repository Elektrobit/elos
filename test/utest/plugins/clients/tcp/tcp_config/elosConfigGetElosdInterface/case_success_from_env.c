// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdInterface_utest.h"

#define MOCK_INTERFACE "127.0.0.1"

int elosTestElosConfigGetElosdInterfaceSuccessFromEnvSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"Interface\": \"" MOCK_INTERFACE
        "\""
        "  }"
        "}",
        true, config);
    *state = config;
    return 0;
}

int elosTestElosConfigGetElosdInterfaceSuccessFromEnvTeardown(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdInterfaceSuccessFromEnv(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;

    TEST("elosTcpConfigGetInterface");
    SHOULD("%s", "get the elos interface from a config");

    elosPlugin_t plugin = {.useEnv = true, .config = config};

    const char *returnValue = elosTcpConfigGetInterface(&plugin);
    assert_string_equal(returnValue, MOCK_INTERFACE);
}
