// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeErrParamPluginManagerNullSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosClientManagerInitializeErrParamPluginManagerNullTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerInitializeErrParamPluginManagerNull(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosClientManagerParam_t const testParam = {
        .pluginManager = NULL,
        .config = test->mockConfig,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("not initialize client manager since param pluginmanager is null");

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
