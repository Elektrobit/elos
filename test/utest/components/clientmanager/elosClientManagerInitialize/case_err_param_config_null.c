// SPDX-License-Identifier: MIT

#include "elosClientManagerInitialize_utest.h"

int elosTestElosClientManagerInitializeErrParamConfigNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerInitializeErrParamConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerInitializeErrParamConfigNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientManagerParam_t const testParam = {
        .pluginManager = &test->pluginmanager,
        .config = NULL,
    };

    TEST("elosClientManagerInitialize");
    SHOULD("not initialize client manager since param config is null");

    result = elosClientManagerInitialize(&test->clientmanager, &testParam);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
