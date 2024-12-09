// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosStorageManagerDeleteMembers_utest.h"

int elosTestElosStorageManagerDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerDeleteMembersSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuVec_t *vector;
    samconfConfig_t *mockConfig = NULL;

    ret = samconfConfigNew(&mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = mockConfig,
    };

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    TEST("elosStorageManagerDeleteMembers");
    SHOULD("delete storageManager members successfully");

    result = elosStorageManagerDeleteMembers(&test->storagemanager);

    assert_int_equal(result, SAFU_RESULT_OK);
    vector = &test->storagemanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, 0);

    elosMockConfigCleanup(mockConfig);
}
