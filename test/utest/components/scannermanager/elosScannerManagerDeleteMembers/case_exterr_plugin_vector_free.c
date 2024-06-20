// SPDX-License-Identifier: MIT

#include "elosScannerManagerDeleteMembers_utest.h"
#include "safu/mock_vector.h"

int elosTestElosScannerManagerDeleteMembersExtErrPluginVectorFreeSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;
    safuResultE_t result = SAFU_RESULT_FAILED;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    elosScannerManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    test->pluginmanager.config = test->mockConfig;
    test->pluginmanager.state = PLUGINMANAGER_STATE_INITIALIZED;
    test->pluginmanager.nextId = 1;

    result = elosScannerManagerInitialize(&test->scannermanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosScannerManagerDeleteMembersExtErrPluginVectorFreeTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);
    return 0;
}

void elosTestElosScannerManagerDeleteMembersExtErrPluginVectorFree(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerDeleteMembers");
    SHOULD("return SAFU_RESULT_FAILED since freeing vector fails");

    MOCK_FUNC_AFTER_CALL(safuVecFree, 0);
    expect_value(__wrap_safuVecFree, vec, &test->scannermanager.pluginControlPtrVector);
    will_return(__wrap_safuVecFree, -1);

    result = elosScannerManagerDeleteMembers(&test->scannermanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
