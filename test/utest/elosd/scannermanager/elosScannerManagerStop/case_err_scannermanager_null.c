// SPDX-License-Identifier: MIT

#include "elosScannerManagerStop_utest.h"

int elosTestElosScannerManagerStopErrScannerManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerStopErrScannerManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerStopErrScannerManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerStop");
    SHOULD("not stop scannerManager since scannermanager input is null");

    result = elosScannerManagerStop(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
