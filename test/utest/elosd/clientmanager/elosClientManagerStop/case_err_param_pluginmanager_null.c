// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"

int elosTestElosClientManagerStopErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerStopErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerStopErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->clientmanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerStop");
    SHOULD("not stop client manager since client manager pluginmanager is null");

    result = elosClientManagerStop(&test->clientmanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
