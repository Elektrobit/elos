// SPDX-License-Identifier: MIT

#include "elosStorageManagerDeleteMembers_utest.h"

int elosTestElosStorageManagerDeleteMembersErrParamPluginManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerDeleteMembersErrParamPluginManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerDeleteMembersErrParamPluginManagerNull(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->storagemanager.pluginManager = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerDeleteMembers");
    SHOULD("not initialize storage manager since storage manager pluginmanager is null");

    result = elosStorageManagerDeleteMembers(&test->storagemanager);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
