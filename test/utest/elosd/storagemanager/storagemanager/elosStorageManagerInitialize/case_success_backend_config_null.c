// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

#define _TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
        }\
    }\
}\
"
samconfConfigStatusE_t elosGetEmptyMockConfig(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *testJobj = NULL;

    testJobj = json_tokener_parse(_TEST_CONFIG);

    result = elosUtilCreateMockConfig(testJobj, false, config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    json_object_put(testJobj);

    return result;
}

int elosTestElosStorageManagerInitializeSuccessBackendConfigNullSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetEmptyMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosStorageManagerInitializeSuccessBackendConfigNullTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosStorageManagerDeleteMembers(&test->storagemanager);
    elosMockConfigCleanup(test->mockConfig);

    return 0;
}

void elosTestElosStorageManagerInitializeSuccessBackendConfigNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    safuVec_t *vector = NULL;

    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("return SAFU_RESULT_FAILED since backend config is not set");

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);
    assert_int_equal(result, SAFU_RESULT_OK);
    vector = &test->storagemanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_null(test->storagemanager.config);
}
