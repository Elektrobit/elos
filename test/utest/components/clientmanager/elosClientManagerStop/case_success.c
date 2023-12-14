// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"
#include "mock_pluginmanager_unload.h"

int elosTestElosClientManagerStopSuccessSetup(void **state) {
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

int elosTestElosClientManagerStopSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerStopSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerStop");
    SHOULD("stops client manager succesfully");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerUnload, 0);
    expect_any(elosPluginManagerUnload, pluginManager);
    expect_any(elosPluginManagerUnload, controlPtrVector);
    will_return(elosPluginManagerUnload, SAFU_RESULT_OK);

    result = elosClientManagerStop(&test->clientmanager);

    assert_int_equal(result, SAFU_RESULT_OK);
}
