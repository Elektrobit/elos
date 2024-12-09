// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosClientManagerInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char *expectedValue = "/usr/local/lib/elos/client";
    safuVec_t *vector;

    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("initialize connection clientManager successfully");

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(test->clientmanager.searchPath, expectedValue);
    vector = &test->clientmanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_string_equal(test->clientmanager.config->key, "ClientInputs");
}
