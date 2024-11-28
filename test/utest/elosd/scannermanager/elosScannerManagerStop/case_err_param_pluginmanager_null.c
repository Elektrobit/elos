// SPDX-License-Identifier: MIT

#include "elosScannerManagerStop_utest.h"

int elosTestElosScannerManagerStopErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerStopErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerStopErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->scannermanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStop");
    SHOULD("not stop scanner manager since scanner manager pluginmanager is null");

    result = elosScannerManagerStop(&test->scannermanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
