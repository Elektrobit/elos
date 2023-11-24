// SPDX-License-Identifier: MIT

#include "elosClientManagerDeleteMembers_utest.h"

int elosTestElosClientManagerDeleteMembersErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerDeleteMembersErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerDeleteMembersErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->clientmanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerDeleteMembers");
    SHOULD("not initialize client manager since client manager pluginmanager is null");

    result = elosClientManagerDeleteMembers(&test->clientmanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
