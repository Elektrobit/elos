// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosUnixConfigGetElosdConnectionLimit_utest.h"

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromConfigSetup(void **state) {
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

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromConfig(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    TEST("elosUnixConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit environment option");

    elosPlugin_t plugin = {.useEnv = true, .config = config};

    int32_t limit = elosUnixConfigGetConnectionLimit(&plugin);
    assert_int_equal(42, limit);
}
