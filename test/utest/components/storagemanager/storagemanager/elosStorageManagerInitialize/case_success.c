// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeSuccessSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosStorageManagerInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosStorageManagerDeleteMembers(&test->storagemanager);
    return 0;
}

void elosTestElosStorageManagerInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuVec_t *vector;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("initialize connection storageManager successfully");

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(test->storagemanager.searchPath, ELOSD_BACKEND_PATH);
    vector = &test->storagemanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
}
