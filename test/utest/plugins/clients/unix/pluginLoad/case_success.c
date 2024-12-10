// SPDX-License-Identifier: MIT

#include <safu/result.h>
#include <samconf/samconf_types.h>

#include "pluginLoad_utest.h"

int elosTestPluginLoadSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestPluginLoadSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPluginLoadSuccess(UNUSED void **state) {
    TEST("pluginLoad");
    SHOULD("load the plugin successfully");

    samconfConfig_t pluginConfig = {
        .key = "unix",
    };
    elosPlugin_t plugin = {.config = &pluginConfig};

    safuResultE_t result = elosPluginConfig.load(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);

    elosPluginConfig.unload(&plugin);
}
