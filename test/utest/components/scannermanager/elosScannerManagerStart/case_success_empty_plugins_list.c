// SPDX-License-Identifier: MIT

#include "elosScannerManagerStart_utest.h"
#include "mock_pluginmanager_load.h"

#define EMPTY_PLUGINS_LIST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"UseEnv\": false,\
            \"Scanner\": {\
                \"PluginSearchPath\": \"/usr/local/lib/elos/scanner\",\
                \"Plugins\": { }\
            }\
        }\
    }\
}"

int elosTestElosScannerManagerStartSuccessEmptyPluginsListSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuResultE_t result = SAFU_RESULT_FAILED;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig, EMPTY_PLUGINS_LIST_CONFIG);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    elosScannerManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    test->pluginmanager.config = test->mockConfig;
    test->pluginmanager.state = PLUGINMANAGER_STATE_INITIALIZED;
    test->pluginmanager.nextId = 1;

    result = elosScannerManagerInitialize(&test->scannermanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosScannerManagerStartSuccessEmptyPluginsListTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);

    return 0;
}

void elosTestElosScannerManagerStartSuccessEmptyPluginsList(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStart");
    SHOULD("start scanner manager succesfully even with empty plugins list");

    result = elosScannerManagerStart(&test->scannermanager);

    assert_int_equal(result, SAFU_RESULT_OK);
}
