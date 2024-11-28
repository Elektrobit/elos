// SPDX-License-Identifier: MIT

#include "mock_pluginmanager_entry.h"

MOCK_FUNC_BODY(elosPluginManagerEntryAdd, safuResultE_t, elosPluginManager_t *pluginManager,
               elosPluginControlParam_t const *param, elosPluginId_t *id) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryAdd)) {
        check_expected_ptr(pluginManager);
        check_expected_ptr(param);
        check_expected_ptr(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryAdd)(pluginManager, param, id);
}

MOCK_FUNC_BODY(elosPluginManagerEntryGet, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id,
               elosPluginControl_t **plugin) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryGet)) {
        check_expected_ptr(pluginManager);
        check_expected(id);
        check_expected_ptr(plugin);
        *plugin = mock_ptr_type(struct elosPluginControl *);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryGet)(pluginManager, id, plugin);
}

MOCK_FUNC_BODY(elosPluginManagerEntryLoad, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryLoad)) {
        check_expected_ptr(pluginManager);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryLoad)(pluginManager, id);
}

MOCK_FUNC_BODY(elosPluginManagerEntryUnload, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryUnload)) {
        check_expected_ptr(pluginManager);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryUnload)(pluginManager, id);
}

MOCK_FUNC_BODY(elosPluginManagerEntryStart, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryStart)) {
        check_expected_ptr(pluginManager);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryStart)(pluginManager, id);
}

MOCK_FUNC_BODY(elosPluginManagerEntryStop, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryStop)) {
        check_expected_ptr(pluginManager);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryStop)(pluginManager, id);
}

MOCK_FUNC_BODY(elosPluginManagerEntryRemove, safuResultE_t, elosPluginManager_t *pluginManager,
               elosPluginId_t pluginId) {
    if (MOCK_IS_ACTIVE(elosPluginManagerEntryRemove)) {
        check_expected_ptr(pluginManager);
        check_expected(pluginId);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginManagerEntryRemove)(pluginManager, pluginId);
}
