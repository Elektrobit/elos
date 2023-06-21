// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectJobjNullErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventToJsonObjectJobjNullErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventToJsonObjectJobjNullError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t sourceEvent = {0};

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED if 'to' is null");

    result = elosEventToJsonObject(NULL, &sourceEvent);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
