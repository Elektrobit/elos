// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int elosTestElosEventCloneErrEventDestSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventCloneErrEventDestTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventCloneErrEventDest(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t testSource = {0};

    TEST("elosEventClone");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the event destination is NULL");

    result = elosEventClone(NULL, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
