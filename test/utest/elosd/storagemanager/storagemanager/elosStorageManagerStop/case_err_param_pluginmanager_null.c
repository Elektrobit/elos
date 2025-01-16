// SPDX-License-Identifier: MIT

#include "elosStorageManagerStop_utest.h"

int elosTestElosStorageManagerStopErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerStopErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerStopErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->storagemanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerStop");
    SHOULD("not stop storage manager since storage manager pluginmanager is null");

    result = elosStorageManagerStop(&test->storagemanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
