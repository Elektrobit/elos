// SPDX-License-Identifier: MIT

#include "elosEventVectorDelete_utest.h"

int elosTestElosEventVectorDeleteSuccessSetup(void **state) {
    elosUteststateT_t *testState = *state;
    const elosEvent_t testEvent = {
        .payload = "payload",
    };
    elosEvent_t testEventPush = {0};
    safuResultE_t result = elosEventDeepCopy(&testEventPush, &testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
    elosEventVectorNew(&testState->eventVector, 1);
    safuVecPush(testState->eventVector, &testEventPush);

    return 0;
}

int elosTestElosEventVectorDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorDeleteSuccess(void **state) {
    elosUteststateT_t *testState = *state;

    TEST("elosEventVectorDelete");
    SHOULD("%s", "successfully free the event vector");

    elosEventVectorDelete(testState->eventVector);

    assert_ptr_not_equal(testState->eventVector, NULL);
}
