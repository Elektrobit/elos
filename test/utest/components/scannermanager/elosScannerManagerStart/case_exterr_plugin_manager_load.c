// SPDX-License-Identifier: MIT

#include "elosScannerManagerStart_utest.h"
#include "mock_pluginmanager_load.h"

int elosTestElosScannerManagerStartExtErrPluginManagerLoadSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuResultE_t result = SAFU_RESULT_FAILED;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
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

int elosTestElosScannerManagerStartExtErrPluginManagerLoadTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);

    return 0;
}

void elosTestElosScannerManagerStartExtErrPluginManagerLoad(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStart");
    SHOULD("return SAFU_RESULT_FAILED since plugin manager load fails");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerLoad, 0);
    expect_any(elosPluginManagerLoad, pluginManager);
    expect_value(elosPluginManagerLoad, type, PLUGIN_TYPE_SCANNER);
    expect_any(elosPluginManagerLoad, moduleConfig);
    expect_any(elosPluginManagerLoad, pluginSearchPath);
    expect_any(elosPluginManagerLoad, controlPtrVector);
    will_return(elosPluginManagerLoad, SAFU_RESULT_FAILED);

    result = elosScannerManagerStart(&test->scannermanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
