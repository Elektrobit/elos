// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdPort_utest.h"

int elosTestElosConfigGetElosdPortSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"Port\": 42"
        "  }"
        "}",
        true, config);
    *state = config;

    return 0;
}

int elosTestElosConfigGetElosdPortSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdPortSuccessFromConfig(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;

    TEST("elosTcpConfigGetPort");
    SHOULD("%s", "get the elos port from the config");

    elosPlugin_t plugin = {.useEnv = true, .config = config};

    int port = elosTcpConfigGetPort(&plugin);
    assert_int_equal(42, port);
}
