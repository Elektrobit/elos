// SPDX-License-Identifier: MIT

#include "elosScannerManagerInitialize_utest.h"

int elosTestElosScannerManagerInitializeErrParamConfigNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerInitializeErrParamConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerInitializeErrParamConfigNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosScannerManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = NULL,
    };

    TEST("elosScannerManagerInitialize");
    SHOULD("not initialize scanner manager since param config is null");

    result = elosScannerManagerInitialize(&test->scannermanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
