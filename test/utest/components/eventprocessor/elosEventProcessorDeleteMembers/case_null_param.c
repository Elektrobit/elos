// SPDX-License-Identifier: MIT

#include "elosEventProcessorDeleteMembers_utest.h"

int elosTestEloEventProcessorDeleteMembersNullParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorDeleteMembersNullParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorDeleteMembersNullParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventProcessorDeleteMembers (invalid parameters)");
    SHOULD("%s", "return SAFU_RESULT_OK with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventProcessorDeleteMembers(NULL);
    assert_true(result == SAFU_RESULT_OK);
}
