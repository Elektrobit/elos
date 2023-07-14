// SPDX-License-Identifier: MIT
#include "elosEventDeleteMembers_utest.h"

int elosTestElosEventDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeleteMembersSuccess(UNUSED void **state) {
    elosEvent_t testEvent = {0};
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventDeleteMembers");
    SHOULD("%s", "free the struct members of an elosEvent_t");

    // This initialization block is here with the last line commented out due to -fsanitizer false positive shenanigans
    testEvent.payload = strdup("testabc");
    testEvent.hardwareid = strdup("hardwareTest");
    testEvent.source.fileName = strdup("testFileName");
    // testEvent->source->appName = strdup("testAppName");

    result = elosEventDeleteMembers(&testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
}
