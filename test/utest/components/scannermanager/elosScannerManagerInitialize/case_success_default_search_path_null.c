// SPDX-License-Identifier: MIT

#include "elosScannerManagerInitialize_utest.h"

int elosTestElosScannerManagerInitializeSuccessDefaultSearchPathSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosScannerManagerInitializeSuccessDefaultSearchPathTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);

    return 0;
}

void elosTestElosScannerManagerInitializeSuccessDefaultSearchPath(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char *expectedValue = "/usr/local/lib/elos/scanner";
    safuVec_t *vector;

    elosScannerManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = test->mockConfig,
    };

    TEST("elosScannerManagerInitialize");
    SHOULD("return SAFU_RESULT_OK since search path is set to default value");

    result = elosScannerManagerInitialize(&test->scannermanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(test->scannermanager.searchPath, expectedValue);
    vector = &test->scannermanager.pluginControlPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(elosPluginControl_t *));
    assert_string_equal(test->scannermanager.config->key, "Scanner");
}
