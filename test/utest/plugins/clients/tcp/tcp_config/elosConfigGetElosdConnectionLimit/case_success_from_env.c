// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"ConnectionLimit\": 42"
        "  }"
        "}",
        true, config);
    *state = config;

    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitSuccessFromConfig(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;

    TEST("elosTcpConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit environment option");

    elosPlugin_t plugin = {.useEnv = true, .config = config};

    int32_t limit = elosTcpConfigGetConnectionLimit(&plugin);
    assert_int_equal(42, limit);
}
