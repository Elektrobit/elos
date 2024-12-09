// SPDX-License-Identifier: MIT

#include "elosStorageManagerDeleteMembers_utest.h"
#include "safu/mock_vector.h"

int elosTestElosStorageManagerDeleteMembersExtErrPluginVectorFreeSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerDeleteMembersExtErrPluginVectorFreeTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosStorageManagerDeleteMembers(&test->storagemanager);
    return 0;
}

void elosTestElosStorageManagerDeleteMembersExtErrPluginVectorFree(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
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
    SHOULD("return SAFU_RESULT_FAILED since freeing vector fails");

    MOCK_FUNC_AFTER_CALL(safuVecFree, 0);
    expect_value(__wrap_safuVecFree, vec, &test->storagemanager.pluginControlPtrVector);
    will_return(__wrap_safuVecFree, -1);

    result = elosStorageManagerDeleteMembers(&test->storagemanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    elosMockConfigCleanup(mockConfig);
}
