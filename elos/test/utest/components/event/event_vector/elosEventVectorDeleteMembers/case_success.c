// SPDX-License-Identifier: MIT

#include "elosEventVectorDeleteMembers_utest.h"

int elosTestElosEventVectorDeleteMembersSuccessSetup(void **state) {
    elosUteststateT_t *testState = *state;
    const elosEvent_t testEvent = {
        .payload = "payload",
    };
    elosEvent_t testEventPush = {0};
    safuResultE_t result = elosEventDeepCopy(&testEventPush, &testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
    elosEventVectorInitialize(&testState->eventVector, 1);
    safuVecPush(&testState->eventVector, &testEventPush);
    return 0;
}

int elosTestElosEventVectorDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorDeleteMembersSuccess(void **state) {
    elosUteststateT_t *testState = *state;

    TEST("elosEventVectorDeleteMembers");
    SHOULD("%s", "successfully free the event vector members");

    elosEventVectorDeleteMembers(&testState->eventVector);

    assert_ptr_equal(testState->eventVector.data, NULL);
    assert_int_equal(testState->eventVector.elementCount, 0);
    assert_int_equal(testState->eventVector.elementSize, 0);
    assert_int_equal(testState->eventVector.memorySize, 0);
}
