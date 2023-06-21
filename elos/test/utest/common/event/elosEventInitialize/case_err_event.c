// SPDX-License-Identifier: MIT
#include "elosEventInitialize_utest.h"

int elosTestElosEventInitializeErrEventSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventInitializeErrEventTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventInitializeErrEvent(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the event parameter is NULL");

    result = elosEventInitialize(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
