// SPDX-License-Identifier: MIT

#include "elosScannerManagerStart_utest.h"

int elosTestElosScannerManagerStartErrScannerManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerStartErrScannerManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerStartErrScannerManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStart");
    SHOULD("not start scannerManager since scannermanager input is null");

    result = elosScannerManagerStart(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
