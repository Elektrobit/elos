// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeErrClientManagerNullSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosClientManagerInitializeErrClientManagerNullTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerInitializeErrClientManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("not initialize clientManager since null clientManager parameter");

    result = elosClientManagerInitialize(NULL, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
