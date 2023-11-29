// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <json-c/json.h>
#include <json-c/json_tokener.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/config/config.h"
#include "elos/pluginmanager/pluginmanager.h"
#include "elos/storagemanager/storagemanager.h"

typedef struct elosUnitTestState {
    elosStorageManager_t storagemanager;
    elosPluginManager_t pluginmanager;
    samconfConfig_t *mockConfig;
} elosUnitTestState_t;

samconfConfigStatusE_t elosTestCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);
samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config);
int elosMockConfigCleanup(samconfConfig_t *config);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosStorageManagerStartSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosStorageManagerStartErrStorageManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosStorageManagerStartExtErrPluginManagerLoad)
