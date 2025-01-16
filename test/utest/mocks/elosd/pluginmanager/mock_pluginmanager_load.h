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

MOCK_FUNC_PROTOTYPE(elosPluginManagerLoad, safuResultE_t, elosPluginManager_t *pluginManager, elosPluginTypeE_t type,
                    samconfConfig_t const *moduleConfig, char const *pluginSearchPath,
                    elosPluginControlPtrVector_t *controlPtrVector)
