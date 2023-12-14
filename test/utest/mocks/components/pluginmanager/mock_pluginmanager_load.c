// SPDX-License-Identifier: MIT

#include "mock_pluginmanager_load.h"

MOCK_FUNC_BODY(elosPluginManagerLoad, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginTypeE_t type,
               samconfConfig_t const *moduleConfig, char const *pluginSearchPath,
               elosPluginControlPtrVector_t *controlPtrVector) {
    if (MOCK_IS_ACTIVE(elosPluginManagerLoad)) {
        check_expected_ptr(pluginManager);
        check_expected(type);
        check_expected_ptr(moduleConfig);
        check_expected_ptr(pluginSearchPath);
        check_expected_ptr(controlPtrVector);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerLoad)(pluginManager, type, moduleConfig, pluginSearchPath, controlPtrVector);
}
