// SPDX-License-Identifier: MIT

#include "elosStorageManagerInitialize_utest.h"

int elosTestElosStorageManagerInitializeErrParamConfigNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerInitializeErrParamConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerInitializeErrParamConfigNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosStorageManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = NULL,
    };

    TEST("elosStorageManagerInitialize");
    SHOULD("not initialize storage manager since param config is null");

    result = elosStorageManagerInitialize(&test->storagemanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
