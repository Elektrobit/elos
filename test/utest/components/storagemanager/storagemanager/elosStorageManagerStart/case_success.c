// SPDX-License-Identifier: MIT

#include "elosStorageManagerStart_utest.h"
#include "mock_pluginmanager_load.h"

int elosTestElosStorageManagerStartSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuResultE_t result = SAFU_RESULT_FAILED;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    test->pluginmanager.config = test->mockConfig;
    test->pluginmanager.state = PLUGINMANAGER_STATE_INITIALIZED;
    test->pluginmanager.nextId = 1;

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosStorageManagerStartSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosStorageManagerDeleteMembers(&test->storagemanager);

    return 0;
}

void elosTestElosStorageManagerStartSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerStart");
    SHOULD("start storage manager succesfully");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerLoad, 0);
    expect_any(elosPluginManagerLoad, pluginManager);
    expect_value(elosPluginManagerLoad, type, PLUGIN_TYPE_STORAGEBACKEND);
    expect_any(elosPluginManagerLoad, moduleConfig);
    expect_any(elosPluginManagerLoad, pluginSearchPath);
    expect_any(elosPluginManagerLoad, controlPtrVector);
    will_return(elosPluginManagerLoad, SAFU_RESULT_OK);

    result = elosStorageManagerStart(&test->storagemanager);

    assert_int_equal(result, SAFU_RESULT_OK);
}
