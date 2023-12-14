// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeSuccessDefaultSearchPathSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosClientManagerInitializeSuccessDefaultSearchPathTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerInitializeSuccessDefaultSearchPath(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char *expectedValue = "/usr/local/lib/elos/client";
    safuVec_t *vector;

    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("return SAFU_RESULT_OK since search path is set to default value");

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(test->clientmanager.searchPath, expectedValue);
    vector = &test->clientmanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_string_equal(test->clientmanager.config->key, "ClientInputs");
}
