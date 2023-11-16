// SPDX-License-Identifier: MIT

#include <safu/result.h>
#include <samconf/samconf_types.h>

#include "pluginUnload_utest.h"

int elosTestPluginUnloadSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestPluginUnloadSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPluginUnloadSuccess(UNUSED void **state) {
    TEST("pluginUnload");
    SHOULD("unload the plugin successfully");

    samconfConfig_t pluginConfig = {
        .key = "tcp",
    };
    elosPlugin_t plugin = {.config = &pluginConfig};

    plugin.state = PLUGIN_STATE_LOADED;
    safuResultE_t result = elosPluginConfig.unload(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);
}
