// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeExtErrClientConfigNullSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosClientManagerInitializeExtErrClientConfigNullTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerInitializeExtErrClientConfigNull(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t *vector;

    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("return SAFU_RESULT_FAILED since client config is not set");

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_any(__wrap_samconfConfigGet, root);
    expect_string(__wrap_samconfConfigGet, path, ELOS_CONFIG_ROOT "ClientInputs/");
    expect_any(__wrap_samconfConfigGet, result);
    will_set_parameter(__wrap_samconfConfigGet, result, NULL);
    will_return(__wrap_samconfConfigGet, SAMCONF_CONFIG_NOT_FOUND);

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    vector = &test->clientmanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_null(test->clientmanager.config);
}
