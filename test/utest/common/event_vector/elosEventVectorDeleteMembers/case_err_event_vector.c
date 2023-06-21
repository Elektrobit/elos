// SPDX-License-Identifier: MIT

#include "elosEventVectorDeleteMembers_utest.h"

int elosTestElosEventVectorDeleteMembersErrEventVectorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorDeleteMembersErrEventVectorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorDeleteMembersErrEventVector(UNUSED void **state) {
    TEST("elosEventVectorDeleteMembers");
    SHOULD("%s", "do nothing when eventVector is null");
    elosEventVectorDeleteMembers(NULL);
}