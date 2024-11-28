// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <json-c/json.h>
#include <json-c/json_tokener.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "elos/config/config.h"
#include "elos/pluginmanager/types.h"
#include "elos/scannermanager/scannermanager.h"

typedef struct elosUnitTestState {
    elosScannerManager_t scannermanager;
    elosPluginManager_t pluginmanager;
    samconfConfig_t *mockConfig;
} elosUnitTestState_t;

samconfConfigStatusE_t elosUtilCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);
samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config);
int elosMockConfigCleanup(samconfConfig_t *config);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeSuccessDefaultSearchPath)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeSuccessScannerConfigNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeErrScannerManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeErrInitParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeErrParamConfigNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeErrParamPluginManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerInitializeExtErrPluginVectorCreate)
