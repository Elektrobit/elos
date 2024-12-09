// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeSuccessDefaultSearchPathSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosStorageManagerInitializeSuccessDefaultSearchPathTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosStorageManagerDeleteMembers(&test->storagemanager);
    elosMockConfigCleanup(test->mockConfig);

    return 0;
}

void elosTestElosStorageManagerInitializeSuccessDefaultSearchPath(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t *vector = NULL;

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("return SAFU_RESULT_OK and fallback to compile default for plugin search path");

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);

    assert_string_equal(test->storagemanager.searchPath, ELOSD_BACKEND_PATH);
    assert_int_equal(result, SAFU_RESULT_OK);
    vector = &test->storagemanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_string_equal(test->storagemanager.config->key, "EventLogging");
}
