// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_PLUGIN_MANAGER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/pluginmanager/pluginmanager.h"

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryAdd, safuResultE_t, elosPluginManager_t *pluginManager,
                    elosPluginControlParam_t const *param, elosPluginId_t *id)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryGet, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id,
                    elosPluginControl_t **plugin)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryLoad, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryUnload, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryStart, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryStop, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginManagerEntryRemove, safuResultE_t, elosPluginManager_t *pluginManager,
                    elosPluginId_t pluginId)
