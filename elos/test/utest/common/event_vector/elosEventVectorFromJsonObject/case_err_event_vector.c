// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"

int elosTestElosEventVectorFromJsonObjectErrEventVectorSetup(void **state) {
    elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(state);
    return 0;
}

int elosTestElosEventVectorFromJsonObjectErrEventVectorTeardown(void **state) {
    elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(state);
    return 0;
}

void elosTestElosEventVectorFromJsonObjectErrEventVector(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVector is null");

    result = elosEventVectorFromJsonObject(testState->eventVecJobj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
