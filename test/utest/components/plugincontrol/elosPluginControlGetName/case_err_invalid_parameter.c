// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <samconf/samconf.h>

#include "elos/plugincontrol/types.h"
#include "elosPluginControlGetName_utest.h"

int elosTestElosPluginControlGetNameErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlGetNameErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

static void _testElosPluginControlGetNameErrInvalidParameterParam(const char *parameterUnderTest, elosPlugin_t *plugin,
                                                                  const char **name) {
    PARAM("%s", parameterUnderTest);
    safuResultE_t result = elosPluginControlGetName(plugin, name);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

void elosTestElosPluginControlGetNameErrInvalidParameter(UNUSED void **state) {
    elosPlugin_t plugin = {0};
    const char *pluginName = NULL;

    TEST("elosPluginControlGetName");
    SHOULD("%s", "fail if one of the provided parameters is invalid");

    _testElosPluginControlGetNameErrInvalidParameterParam("plugin == NULL", NULL, &pluginName);

    plugin.config = NULL;
    _testElosPluginControlGetNameErrInvalidParameterParam("plugin.config == NULL", &plugin, &pluginName);

    samconfConfig_t config = {0};
    plugin.config = &config;
    _testElosPluginControlGetNameErrInvalidParameterParam("pluginName == NULL", &plugin, NULL);
}
