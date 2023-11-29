// SPDX-License-Identifier: MIT

#include "elosStorageManagerStop_utest.h"

int elosTestElosStorageManagerStopErrStorageManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerStopErrStorageManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerStopErrStorageManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerStop");
    SHOULD("not stop storageManager since storagemanager input is null");

    result = elosStorageManagerStop(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
