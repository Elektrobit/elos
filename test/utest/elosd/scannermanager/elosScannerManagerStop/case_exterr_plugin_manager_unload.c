// SPDX-License-Identifier: MIT

#include "elosScannerManagerStop_utest.h"
#include "mock_pluginmanager_unload.h"

int elosTestElosScannerManagerStopExtErrPluginManagerUnloadSetup(void **state) {
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

int elosTestElosScannerManagerStopExtErrPluginManagerUnloadTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);

    return 0;
}

void elosTestElosScannerManagerStopExtErrPluginManagerUnload(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStop");
    SHOULD("return SAFU_RESULT_FAILED since plugin manager unload fails");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerUnload, 0);
    expect_any(elosPluginManagerUnload, pluginManager);
    expect_any(elosPluginManagerUnload, controlPtrVector);
    will_return(elosPluginManagerUnload, SAFU_RESULT_FAILED);

    result = elosScannerManagerStop(&test->scannermanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
