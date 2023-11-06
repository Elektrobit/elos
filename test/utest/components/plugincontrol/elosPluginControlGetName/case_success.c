// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <samconf/samconf.h>

#include "elosPluginControlGetName_utest.h"

int elosTestElosPluginControlGetNameSuccessSetup(UNUSED void **state) {
    elosPluginTest_t *pluginTest = malloc(sizeof(elosPluginTest_t));
    assert_non_null(pluginTest);

    samconfConfig_t *config = NULL;
    samconfConfigStatusE_t configStatus = samconfConfigNew(&config);
    assert_int_equal(configStatus, SAMCONF_CONFIG_OK);
    config->parent = NULL;
    config->key = strdup("TestPluginName");
    config->type = SAMCONF_CONFIG_VALUE_OBJECT;
    config->children = NULL;
    config->childCount = 0;

    pluginTest->plugin.state = PLUGIN_STATE_INVALID;
    pluginTest->plugin.config = config;
    *state = pluginTest;
    return 0;
}

int elosTestElosPluginControlGetNameSuccessTeardown(UNUSED void **state) {
    elosPluginTest_t *pluginTest = *((elosPluginTest_t **)state);

    samconfConfigDelete((samconfConfig_t *)pluginTest->plugin.config);
    free(pluginTest);

    return 0;
}

void elosTestElosPluginControlGetNameSuccess(UNUSED void **state) {
    const elosPlugin_t *plugin = &(*(elosPluginTest_t **)state)->plugin;
    const char *pluginName = NULL;
    const char *expectedName = "TestPluginName";

    TEST("elosPluginControlGetName");
    SHOULD("%s", "return configuration name of a plugin");

    safuResultE_t result = elosPluginControlGetName(plugin, &pluginName);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(pluginName, expectedName);
}
