// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeErrInitParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerInitializeErrInitParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerInitializeErrInitParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    TEST("elosStorageManagerInitialize");
    SHOULD("not initialize storage manager since param is null");

    result = elosStorageManagerInitialize(&test->storagemanager, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
