// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <stddef.h>

#include "elos/libelosplugin/types.h"
#include "pluginUnload_utest.h"

int elosTestPluginUnloadErrInvalidPluginSetup(UNUSED void **state) {
    return 0;
}

int elosTestPluginUnloadErrInvalidPluginTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPluginUnloadErrInvalidPlugin(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("pluginUnload");
    SHOULD("unloading the plugin fails because plugin is invalid or its state");

    PARAM("plugin is NULL");
    result = elosPluginConfig.unload(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    samconfConfig_t pluginConfig = {
        .key = NULL,
    };
    elosPlugin_t plugin = {.config = &pluginConfig};

    elosPluginStateE_t invalidPluginStates[] = {
        PLUGIN_STATE_ERROR, PLUGIN_STATE_INVALID, PLUGIN_STATE_STARTED, PLUGIN_STATE_UNLOADED, PLUGIN_STATE_INITIALIZED,
    };

    for (size_t i = 0; i < ARRAY_SIZE(invalidPluginStates); i++) {
        plugin.state = invalidPluginStates[i];
        PARAM("plugin.state = %d", plugin.state);
        result = elosPluginConfig.unload(&plugin);
        assert_int_equal(result, SAFU_RESULT_FAILED);
    }
}
