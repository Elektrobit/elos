// SPDX-License-Identifier: MIT

#include "mock_pluginmanager.h"

MOCK_FUNC_BODY(elosPluginManagerInitialize, safuResultE_t, elosPluginManager_t *pluginManager,
               elosPluginManagerParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosPluginManagerInitialize)) {
        check_expected_ptr(pluginManager);
        check_expected_ptr(param);
        pluginManager = mock_ptr_type(elosPluginManager_t *);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerInitialize)(pluginManager, param);
}

MOCK_FUNC_BODY(elosPluginManagerDeleteMembers, safuResultE_t, elosPluginManager_t *pluginManager) {
    if (MOCK_IS_ACTIVE(elosPluginManagerDeleteMembers)) {
        check_expected_ptr(pluginManager);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerDeleteMembers)(pluginManager);
}
