// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <samconf/samconf.h>

#include "elos/plugin/types.h"
#include "elosPluginGetName_utest.h"

int elosTestElosPluginGetNameErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginGetNameErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

static void _testElosPluginGetNameErrInvalidParameterParam(const char *parameterUnderTest, elosPlugin_t *plugin,
                                                           const char **name) {
    PARAM("%s", parameterUnderTest);
    safuResultE_t result = elosPluginGetName(plugin, name);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

void elosTestElosPluginGetNameErrInvalidParameter(UNUSED void **state) {
    elosPlugin_t plugin = {0};
    const char *pluginName = NULL;

    TEST("elosPluginGetName");
    SHOULD("%s", "fail if one of the provided parameters is invalid");

    _testElosPluginGetNameErrInvalidParameterParam("plugin == NULL", NULL, &pluginName);

    plugin.config = NULL;
    _testElosPluginGetNameErrInvalidParameterParam("plugin.config == NULL", &plugin, &pluginName);

    samconfConfig_t config = {0};
    plugin.config = &config;
    _testElosPluginGetNameErrInvalidParameterParam("pluginName == NULL", &plugin, NULL);
}
