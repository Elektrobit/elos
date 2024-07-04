// SPDX-License-Identifier: MIT

#include "elosScannerManagerDeleteMembers_utest.h"

int elosTestElosScannerManagerDeleteMembersErrScannerManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerDeleteMembersErrScannerManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerDeleteMembersErrScannerManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerDeleteMembers");
    SHOULD("not delete members since scannerManager is null");

    result = elosScannerManagerDeleteMembers(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
