// SPDX-License-Identifier: MIT

#include <safu/result.h>
#include <samconf/samconf_types.h>

#include "pluginLoad_utest.h"

int elosTestPluginLoadErrInvalidPluginSetup(UNUSED void **state) {
    return 0;
}

int elosTestPluginLoadErrInvalidPluginTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPluginLoadErrInvalidPlugin(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("pluginLoad");
    SHOULD("load the plugin and fail because of an invalid plugin parameter");

    PARAM("plugin is NULL");
    result = elosPluginConfig.load(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("plugin.config is NULL");
    elosPlugin_t plugin = {.config = NULL};
    result = elosPluginConfig.load(&plugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("plugin.config.key is NULL");
    samconfConfig_t pluginConfig = {
        .key = NULL,
    };
    plugin.config = &pluginConfig;
    result = elosPluginConfig.load(&plugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
