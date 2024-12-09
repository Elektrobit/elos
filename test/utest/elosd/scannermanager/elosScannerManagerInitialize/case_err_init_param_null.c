// SPDX-License-Identifier: MIT

#include "elosScannerManagerInitialize_utest.h"

int elosTestElosScannerManagerInitializeErrInitParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerInitializeErrInitParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerInitializeErrInitParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosScannerManager_t *testscannermanager = NULL;

    TEST("elosScannerManagerInitialize");
    SHOULD("not initialize scanner manager since param is null");

    result = elosScannerManagerInitialize(testscannermanager, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
