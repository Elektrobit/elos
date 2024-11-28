// SPDX-License-Identifier: MIT

#include "elosStorageManagerDeleteMembers_utest.h"

int elosTestElosStorageManagerDeleteMembersErrStorageManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageManagerDeleteMembersErrStorageManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageManagerDeleteMembersErrStorageManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosStorageManagerDeleteMembers");
    SHOULD("not delete members since storageManager is null");

    result = elosStorageManagerDeleteMembers(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
