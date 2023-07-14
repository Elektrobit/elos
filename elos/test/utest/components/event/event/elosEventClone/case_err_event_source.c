// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int elosTestElosEventCloneErrEventSourceSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventCloneErrEventSourceTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventCloneErrEventSource(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t *testDest = NULL;

    TEST("elosEventClone");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the event source is NULL");

    result = elosEventClone(&testDest, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
