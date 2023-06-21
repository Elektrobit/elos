// SPDX-License-Identifier: MIT

#include "elosEventVectorInitialize_utest.h"

int elosTestElosEventVectorInitializeSuccessSetup(void **state) {
    elosUteststateT_t *testState = *state;
    testState->numElements = 3;
    return 0;
}

int elosTestElosEventVectorInitializeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorInitializeSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventVectorInitialize");
    SHOULD("%s", "successfully initialize an event vector");

    PARAM("Initialize vector with elementCount = 0");
    result = elosEventVectorInitialize(&testState->eventVector, 0);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(testState->eventVector.data, NULL);
    assert_int_equal(testState->eventVector.elementCount, 0);
    assert_int_equal(testState->eventVector.elementSize, sizeof(elosEvent_t));
    assert_int_equal(testState->eventVector.memorySize, 0);
    elosEventVectorDeleteMembers(&testState->eventVector);

    PARAM("Initialize vector with elementCount > 0");
    result = elosEventVectorInitialize(&testState->eventVector, testState->numElements);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_not_equal(testState->eventVector.data, NULL);
    assert_int_equal(testState->eventVector.elementCount, 0);
    assert_int_equal(testState->eventVector.elementSize, sizeof(elosEvent_t));
    assert_int_equal(testState->eventVector.memorySize, sizeof(elosEvent_t) * testState->numElements);
    elosEventVectorDeleteMembers(&testState->eventVector);
}
