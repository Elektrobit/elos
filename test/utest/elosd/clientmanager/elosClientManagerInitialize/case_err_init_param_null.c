// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeErrInitParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerInitializeErrInitParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerInitializeErrInitParamNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientManager_t *testclientmanager = NULL;

    TEST("elosClientManagerInitialize");
    SHOULD("not initialize client manager since param is null");

    result = elosClientManagerInitialize(testclientmanager, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
