// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/plugin/plugin.h"

typedef struct elosPluginTestData {
    uint32_t customFuncBits;
} elosPluginTestData_t;

typedef struct elosUnitTestState {
    elosPlugin_t plugin;
    elosPluginTestData_t data;
} elosUnitTestState_t;

#define _CUSTOM_LOAD_BIT   (1 << 0)
#define _CUSTOM_START_BIT  (1 << 1)
#define _CUSTOM_STOP_BIT   (1 << 2)
#define _CUSTOM_UNLOAD_BIT (1 << 3)

extern char const *elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_COUNT];

safuResultE_t elosCustomLoad(elosPluginContext_t *plugin);
safuResultE_t elosCustomStart(elosPluginContext_t *plugin);
safuResultE_t elosCustomStop(elosPluginContext_t *plugin);
safuResultE_t elosCustomUnload(elosPluginContext_t *plugin);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginLoadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginLoadSuccessFuncOverride)
