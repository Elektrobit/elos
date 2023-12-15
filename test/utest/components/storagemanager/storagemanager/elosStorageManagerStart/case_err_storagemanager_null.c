// SPDX-License-Identifier: MIT

#include "elosStorageManagerStart_utest.h"

int elosTestElosStorageManagerStartErrStorageManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerStartErrStorageManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerStartErrStorageManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerStart");
    SHOULD("not start storageManager since storagemanager input is null");

    result = elosStorageManagerStart(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
