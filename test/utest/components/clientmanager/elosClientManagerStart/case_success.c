// SPDX-License-Identifier: MIT

#include "elosClientManagerStart_utest.h"
#include "mock_pluginmanager_load.h"

int elosTestElosClientManagerStartSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuResultE_t result = SAFU_RESULT_FAILED;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    test->pluginmanager.config = test->mockConfig;
    test->pluginmanager.state = PLUGINMANAGER_STATE_INITIALIZED;
    test->pluginmanager.nextId = 1;

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosClientManagerStartSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerStartSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerStart");
    SHOULD("start client manager succesfully");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerLoad, 0);
    expect_any(elosPluginManagerLoad, pluginManager);
    expect_value(elosPluginManagerLoad, type, PLUGIN_TYPE_CLIENTCONNECTION);
    expect_any(elosPluginManagerLoad, moduleConfig);
    expect_any(elosPluginManagerLoad, pluginSearchPath);
    expect_any(elosPluginManagerLoad, controlPtrVector);
    will_return(elosPluginManagerLoad, SAFU_RESULT_OK);

    result = elosClientManagerStart(&test->clientmanager);

    assert_int_equal(result, SAFU_RESULT_OK);
}
