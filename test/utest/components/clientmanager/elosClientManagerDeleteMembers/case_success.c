// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosClientManagerDeleteMembers_utest.h"

int elosTestElosClientManagerDeleteMembersSuccessSetup(void **state) {
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

int elosTestElosClientManagerDeleteMembersSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosClientManagerDeleteMembers(&test->clientmanager);

    return 0;
}

void elosTestElosClientManagerDeleteMembersSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t *vector;

    TEST("elosClientManagerDeleteMembers");
    SHOULD("delete clientManager members successfully");

    result = elosClientManagerDeleteMembers(&test->clientmanager);

    assert_int_equal(result, SAFU_RESULT_OK);
    vector = &test->clientmanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, 0);
}
