// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"

int elosTestElosEventVectorFromJsonObjectErrEventVectorJobjSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorFromJsonObjectErrEventVectorJobjTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorFromJsonObjectErrEventVectorJobj(UNUSED void **state) {
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVecJobj is null");

    result = elosEventVectorFromJsonObject(NULL, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}