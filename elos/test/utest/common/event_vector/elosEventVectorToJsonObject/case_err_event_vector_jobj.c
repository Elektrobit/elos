// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"

int elosTestElosEventVectorToJsonObjectErrEventVectorJobjSetup(void **state) {
    elosEventVectorToJsonObjectUtestCreateEventVector(state);
    return 0;
}

int elosTestElosEventVectorToJsonObjectErrEventVectorJobjTeardown(void **state) {
    elosEventVectorToJsonObjectUtestDeleteEventVector(state);
    return 0;
}

void elosTestElosEventVectorToJsonObjectErrEventVectorJobj(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVecJobj is null");

    result = elosEventVectorToJsonObject(testState->eventVector, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
