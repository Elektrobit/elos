// SPDX-License-Identifier: MIT

#include "elosScannerManagerDeleteMembers_utest.h"

int elosTestElosScannerManagerDeleteMembersErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosScannerManagerDeleteMembersErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosScannerManagerDeleteMembersErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->scannermanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosScannerManagerDeleteMembers");
    SHOULD("fail to delete scanner manager members since pluginmanager is null");

    result = elosScannerManagerDeleteMembers(&test->scannermanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
