// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>

#include "connectionmanager/connectionmanager_types.h"
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
        .key = "unix",
    };
    elosConnectionManager_t *conMan = safuAllocMem(NULL, sizeof(elosConnectionManager_t));
    elosPlugin_t plugin = {
        .config = &pluginConfig,
        .data = conMan,
    };

    safuResultE_t result = elosPluginConfig.unload(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);
}
