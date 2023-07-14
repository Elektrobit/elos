// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"

int elosTestElosEventVectorFromJsonObjectSuccessSetup(void **state) {
    elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(state);
    return 0;
}

int elosTestElosEventVectorFromJsonObjectSuccessTeardown(void **state) {
    elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(state);
    return 0;
}

void elosTestElosEventVectorFromJsonObjectSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "successfully transfer a json object to an event vector");

    result = elosEventVectorFromJsonObject(testState->eventVecJobj, &eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testState->expectedEventVector->elementCount, eventVector->elementCount);

    for (unsigned int idx = 0; idx < eventVector->elementCount; idx++) {
        elosEvent_t *eventEntry01 = (elosEvent_t *)safuVecGet(eventVector, idx);
        elosEvent_t *eventEntry02 = (elosEvent_t *)safuVecGet(testState->expectedEventVector, idx);
        assert_memory_equal(eventEntry01, eventEntry02, sizeof(elosEvent_t));
    }

    elosEventVectorDelete(eventVector);
}
