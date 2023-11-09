// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <sys/eventfd.h>

#include "elosPluginControlLoad_utest.h"

char const *elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_COUNT] = {
    [ELOS_PLUGIN_FUNC_LOAD] = "elosCustomLoad",
    [ELOS_PLUGIN_FUNC_START] = "elosCustomStart",
    [ELOS_PLUGIN_FUNC_STOP] = "elosCustomStop",
    [ELOS_PLUGIN_FUNC_UNLOAD] = "elosCustomUnload",
};

safuResultE_t elosCustomLoad(elosPlugin_t *plugin) {
    elosPluginTestData_t *data = (elosPluginTestData_t *)plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;

    data->customFuncBits |= _CUSTOM_LOAD_BIT;

    return result;
}

safuResultE_t elosCustomStart(elosPlugin_t *plugin) {
    elosPluginTestData_t *data = (elosPluginTestData_t *)plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;

    data->customFuncBits |= _CUSTOM_START_BIT;

    return result;
}

safuResultE_t elosCustomStop(elosPlugin_t *plugin) {
    elosPluginTestData_t *data = (elosPluginTestData_t *)plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;

    data->customFuncBits |= _CUSTOM_STOP_BIT;

    return result;
}

safuResultE_t elosCustomUnload(elosPlugin_t *plugin) {
    elosPluginTestData_t *data = (elosPluginTestData_t *)plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;

    data->customFuncBits |= _CUSTOM_UNLOAD_BIT;

    return result;
}
