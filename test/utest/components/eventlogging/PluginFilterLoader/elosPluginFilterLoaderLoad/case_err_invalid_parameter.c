// SPDX-License-Identifier: MIT

#include <elos/plugincontrol/types.h>
#include <samconf/samconf.h>

#include "elosPluginFilterLoaderLoad_utest.h"

int elosTestElosPluginFilterLoaderLoadErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginFilterLoaderLoadErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

void _testelosPluginFilterLoaderLoadErrInvalidParameterParam(const char *parameterUnderTest, elosPlugin_t *plugin) {
    PARAM("%s", parameterUnderTest);
    safuResultE_t result = elosPluginFilterLoaderLoad(plugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

void elosTestElosPluginFilterLoaderLoadErrInvalidParameter(UNUSED void **state) {
    TEST("elosPluginFilterLoaderLoad");
    SHOULD("%s", "Test if invalid parameter are processed correctly");

    _testelosPluginFilterLoaderLoadErrInvalidParameterParam("plugin is NULL", NULL);

    elosPlugin_t plugin = {.config = NULL};
    _testelosPluginFilterLoaderLoadErrInvalidParameterParam("plugin->config is NULL", &plugin);

    samconfConfig_t config = {0};
    plugin.config = &config;
    plugin.data = NULL;
    _testelosPluginFilterLoaderLoadErrInvalidParameterParam("plugin->data is NULL", &plugin);
}
