// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeErrStorageManagerNullSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosStorageManagerInitializeErrStorageManagerNullTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosStorageManagerDeleteMembers(&test->storagemanager);

    return 0;
}

void elosTestElosStorageManagerInitializeErrStorageManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosStorageManagerParam_t param = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("not initialize storageManager since null storageManager parameter");

    result = elosStorageManagerInitialize(NULL, &param);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
