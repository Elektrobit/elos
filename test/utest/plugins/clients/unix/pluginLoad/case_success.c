// SPDX-License-Identifier: MIT

#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <samconf/test_utils.h>
#include <string.h>

#include "pluginLoad_utest.h"

int elosTestPluginLoadSuccessSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(
        "{"
        "   \"Config\": {"
        "       \"ConnectionLimit\": 42,"
        "       \"path\": \"/tmp/elosd_test.socket\","
        "       \"EventBlacklist\": \".event.messageCode 12 GE\","
        "       \"authorizedProcesses\": ["
        "           \".process.uid 0 EQ .process.gid 0 EQ AND .process.exec '/usr/bin/elosc' STRCMP AND\","
        "           \".process.gid 200 EQ .process.exec '/usr/bin/elosc' STRCMP AND\","
        "           \".process.pid 1 EQ\""
        "       ]"
        "   }"
        "}",
        true, config);
    config->key = strdup("Unix");
    *state = config;
    return 0;
}

int elosTestPluginLoadSuccessTeardown(void **state) {
    samconfConfig_t *config = *state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestPluginLoadSuccess(void **state) {
    samconfConfig_t *config = *state;

    TEST("pluginLoad");
    SHOULD("load the plugin successfully");

    elosPlugin_t plugin = {.config = config};

    safuResultE_t result = elosPluginConfig.load(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);

    elosPluginConfig.unload(&plugin);
}
