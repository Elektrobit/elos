// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <json-c/json.h>
#include <json-c/json_tokener.h>
#include <safu/common.h>
#include <safu/log.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/plugincontrol/plugincontrol.h"

typedef struct elosPluginTestData {
    uint32_t customFuncBits;
} elosPluginTestData_t;

typedef struct elosUnitTestState {
    elosPluginControl_t pluginControl;
    elosPluginTestData_t data;
    samconfConfig_t *mockRootConfig;
    samconfConfig_t const *mockPluginConfig;
} elosUnitTestState_t;

#define _CUSTOM_LOAD_BIT   (1 << 0)
#define _CUSTOM_START_BIT  (1 << 1)
#define _CUSTOM_STOP_BIT   (1 << 2)
#define _CUSTOM_UNLOAD_BIT (1 << 3)

safuResultE_t elosCustomLoad(elosPlugin_t *plugin);
safuResultE_t elosCustomStart(elosPlugin_t *plugin);
safuResultE_t elosCustomStop(elosPlugin_t *plugin);
safuResultE_t elosCustomUnload(elosPlugin_t *plugin);

samconfConfigStatusE_t elosUtilCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);
samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config);
int elosMockConfigCleanup(samconfConfig_t *config);

extern elosPluginConfig_t const elosPluginConfig;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlLoadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlLoadSuccessLocal)
