// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <sys/eventfd.h>

#include "elosPluginControlLoad_utest.h"

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

elosPluginConfig_t const elosPluginConfig = {
    .type = PLUGIN_TYPE_SCANNER,
    .load = elosCustomLoad,
    .unload = elosCustomUnload,
    .start = elosCustomStart,
    .stop = elosCustomStop,
};
