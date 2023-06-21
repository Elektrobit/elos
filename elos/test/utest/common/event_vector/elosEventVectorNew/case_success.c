// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>

#include "elosEventVectorNew_utest.h"

int elosTestElosEventVectorNewSuccessSetup(void **state) {
    elosUteststateT_t *testState = *state;
    testState->eventVector = NULL;
    testState->numElements = 3;
    return 0;
}

int elosTestElosEventVectorNewSuccessTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVectorDelete(testState->eventVector);
    return 0;
}

void elosTestElosEventVectorNewSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventVectorNew");
    SHOULD("%s", "successfully create an event vector");

    PARAM("Initialize vector with elementCount = 0");
    result = elosEventVectorNew(&testState->eventVector, 0);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(testState->eventVector->data, NULL);
    assert_int_equal(testState->eventVector->elementCount, 0);
    assert_int_equal(testState->eventVector->elementSize, sizeof(elosEvent_t));
    assert_int_equal(testState->eventVector->memorySize, 0);
    elosEventVectorDelete(testState->eventVector);
    testState->eventVector = NULL;

    PARAM("Initialize vector with elementCount > 0");
    result = elosEventVectorNew(&testState->eventVector, testState->numElements);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null_msg(testState->eventVector, "eventVector shall be initialized");
    assert_ptr_not_equal(testState->eventVector->data, NULL);
    assert_int_equal(testState->eventVector->elementCount, 0);
    assert_int_equal(testState->eventVector->elementSize, sizeof(elosEvent_t));
    assert_int_equal(testState->eventVector->memorySize, sizeof(elosEvent_t) * testState->numElements);
    elosEventVectorDelete(testState->eventVector);
    testState->eventVector = NULL;
}
