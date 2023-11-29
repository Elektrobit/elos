// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeExtErrBackendConfigNullSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosStorageManagerInitializeExtErrBackendConfigNullTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosStorageManagerDeleteMembers(&test->storagemanager);
    elosMockConfigCleanup(test->mockConfig);

    return 0;
}

void elosTestElosStorageManagerInitializeExtErrBackendConfigNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t *vector = NULL;

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("return SAFU_RESULT_FAILED since backend config is not set");

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_any(__wrap_samconfConfigGet, root);
    expect_string(__wrap_samconfConfigGet, path, ELOS_CONFIG_ROOT "EventLogging/");
    expect_any(__wrap_samconfConfigGet, result);
    will_set_parameter(__wrap_samconfConfigGet, result, NULL);
    will_return(__wrap_samconfConfigGet, SAMCONF_CONFIG_NOT_FOUND);

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    vector = &test->storagemanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_null(test->storagemanager.config);
}
