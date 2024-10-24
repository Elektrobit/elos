// SPDX-License-Identifier: MIT

#include "mock_pluginmanager_remove.h"

MOCK_FUNC_BODY(elosPluginManagerRemove, safuResultE_t, elosPluginManager_t *pluginManager,
               elosPluginControlVector_t *controlPtrVector) {
    if (MOCK_IS_ACTIVE(elosPluginManagerRemove)) {
        check_expected_ptr(pluginManager);
        check_expected_ptr(controlPtrVector);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerRemove)(pluginManager, controlPtrVector);
}
