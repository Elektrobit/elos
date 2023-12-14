// SPDX-License-Identifier: MIT

#include "elosClientManagerDeleteMembers_utest.h"
#include "safu/mock_vector.h"

int elosTestElosClientManagerDeleteMembersExtErrPluginVectorFreeSetup(void **state) {
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

int elosTestElosClientManagerDeleteMembersExtErrPluginVectorFreeTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);
    return 0;
}

void elosTestElosClientManagerDeleteMembersExtErrPluginVectorFree(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerDeleteMembers");
    SHOULD("return SAFU_RESULT_FAILED since freeing vector fails");

    MOCK_FUNC_AFTER_CALL(safuVecFree, 0);
    expect_value(__wrap_safuVecFree, vec, &test->clientmanager.pluginControlPtrVector);
    will_return(__wrap_safuVecFree, -1);

    result = elosClientManagerDeleteMembers(&test->clientmanager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
