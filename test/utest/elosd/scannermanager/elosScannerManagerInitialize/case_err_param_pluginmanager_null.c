// SPDX-License-Identifier: MIT

#include "elosScannerManagerInitialize_utest.h"

int elosTestElosScannerManagerInitializeErrParamPluginManagerNullSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfigStatusE_t ret = SAMCONF_CONFIG_OK;

    ret = samconfConfigNew(&test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    ret = elosGetMockConfig(test->mockConfig);
    assert_int_equal(ret, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosScannerManagerInitializeErrParamPluginManagerNullTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosMockConfigCleanup(test->mockConfig);
    elosScannerManagerDeleteMembers(&test->scannermanager);

    return 0;
}

void elosTestElosScannerManagerInitializeErrParamPluginManagerNull(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosScannerManagerParam_t const testParam = {
        .pluginManager = NULL,
        .config = test->mockConfig,
    };

    TEST("elosScannerManagerInitialize");
    SHOULD("not initialize scanner manager since param pluginmanager is null");

    result = elosScannerManagerInitialize(&test->scannermanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
