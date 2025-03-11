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
    SHOULD("try to unload the plugin and fail because of an invalid plugin");

    PARAM("plugin is NULL");

    result = elosPluginConfig.unload(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
