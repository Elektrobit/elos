// SPDX-License-Identifier: MIT

#include "elosStorageManagerStop_utest.h"
#include "mock_pluginmanager_unload.h"

int elosTestElosStorageManagerStopExtErrPluginManagerUnloadSetup(void **state) {
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

int elosTestElosStorageManagerStopExtErrPluginManagerUnloadTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosStorageManagerDeleteMembers(&test->storagemanager);

    return 0;
}

void elosTestElosStorageManagerStopExtErrPluginManagerUnload(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerStop");
    SHOULD("return SAFU_RESULT_FAILED since plugin manager unload fails");

    MOCK_FUNC_AFTER_CALL(elosPluginManagerUnload, 0);
    expect_any(elosPluginManagerUnload, pluginManager);
    expect_any(elosPluginManagerUnload, controlPtrVector);
    will_return(elosPluginManagerUnload, SAFU_RESULT_FAILED);

    result = elosStorageManagerStop(&test->storagemanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
