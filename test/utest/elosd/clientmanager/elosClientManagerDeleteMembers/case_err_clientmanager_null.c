// SPDX-License-Identifier: MIT

#include "elosClientManagerDeleteMembers_utest.h"

int elosTestElosClientManagerDeleteMembersErrClientManagerNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerDeleteMembersErrClientManagerNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerDeleteMembersErrClientManagerNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosClientManagerDeleteMembers");
    SHOULD("not delete members since clientManager is null");

    result = elosClientManagerDeleteMembers(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
