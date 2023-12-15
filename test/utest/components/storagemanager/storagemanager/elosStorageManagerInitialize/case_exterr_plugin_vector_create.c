// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"
#include "safu/mock_vector.h"

int elosTestElosStorageManagerInitializeExtErrPluginVectorCreateSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);
    return 0;
}

int elosTestElosStorageManagerInitializeExtErrPluginVectorCreateTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosStorageManagerDeleteMembers(&test->storagemanager);

    return 0;
}

void elosTestElosStorageManagerInitializeExtErrPluginVectorCreate(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("return SAFU_RESULT_FAILED since pluginControlPtrVector creation fails");

    MOCK_FUNC_AFTER_CALL(safuVecCreate, 0);
    expect_value(__wrap_safuVecCreate, vec, &test->storagemanager.pluginControlPtrVector);
    expect_value(__wrap_safuVecCreate, elements, 1);
    expect_value(__wrap_safuVecCreate, elementSize, sizeof(elosPluginControl_t *));
    will_return(__wrap_safuVecCreate, -1);

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
