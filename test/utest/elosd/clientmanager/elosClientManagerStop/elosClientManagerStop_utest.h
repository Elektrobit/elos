// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <json-c/json.h>
#include <json-c/json_tokener.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/clientmanager/clientmanager.h"
#include "elos/config/config.h"
#include "elos/pluginmanager/pluginmanager.h"

typedef struct elosUnitTestState {
    elosClientManager_t clientmanager;
    elosPluginManager_t pluginmanager;
    samconfConfig_t *mockConfig;
} elosUnitTestState_t;

samconfConfigStatusE_t elosUtilCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);
samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config);
int elosMockConfigCleanup(samconfConfig_t *config);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopErrClientManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopErrParamPluginManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopExtErrPluginManagerUnload)
