// SPDX-License-Identifier: MIT

#include <safu/result.h>
#include <samconf/samconf_types.h>

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
    SHOULD("try to unload the plugin and fail because of an invalid plugin paramater");

    PARAM("plugin is NULL");
    result = elosPluginConfig.unload(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("plugin.config is NULL");
    elosPlugin_t plugin = {.config = NULL};
    result = elosPluginConfig.unload(&plugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
