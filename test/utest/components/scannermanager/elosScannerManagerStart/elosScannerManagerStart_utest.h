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
#include "elos/scannermanager/scannermanager.h"

#define TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"UseEnv\": false,\
            \"Scanner\": {\
                \"PluginSearchPath\": \"/usr/local/lib/elos/scanner\",\
                \"Plugins\": {\
                    \"ScannerDummy\": {\
                        \"File\": \"scanner_dummy.so\",\
                        \"Run\": \"always\"\
                    },\
                    \"DummyScanner\": {\
                        \"File\": \"scanner_dummy.so\",\
                        \"Run\": \"always\"\
                    }\
                }\
            }\
        }\
    }\
}"

typedef struct elosUnitTestState {
    elosScannerManager_t scannermanager;
    elosPluginManager_t pluginmanager;
    samconfConfig_t *mockConfig;
} elosUnitTestState_t;

samconfConfigStatusE_t elosUtilCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);
samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config, const char *json);
int elosMockConfigCleanup(samconfConfig_t *config);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerStartSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerStartSuccessEmptyPluginsList)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerStartSuccessNoPluginsList)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerStartErrScannerManagerNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosScannerManagerStartExtErrPluginManagerLoad)
