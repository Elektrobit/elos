// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/plugincontrol/plugincontrol.h"

typedef struct elosPluginTestData {
    uint32_t customFuncBits;
} elosPluginTestData_t;

typedef struct elosUnitTestState {
    elosPluginControl_t pluginControl;
    elosPluginTestData_t data;
} elosUnitTestState_t;

#define _CUSTOM_LOAD_BIT   (1 << 0)
#define _CUSTOM_START_BIT  (1 << 1)
#define _CUSTOM_STOP_BIT   (1 << 2)
#define _CUSTOM_UNLOAD_BIT (1 << 3)

safuResultE_t elosCustomLoad(elosPlugin_t *plugin);
safuResultE_t elosCustomStart(elosPlugin_t *plugin);
safuResultE_t elosCustomStop(elosPlugin_t *plugin);
safuResultE_t elosCustomUnload(elosPlugin_t *plugin);

extern elosPluginConfig_t const elosPluginConfig;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlLoadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlLoadSuccessLocal)
