// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"
#include "mock_event_vector.h"

int elosTestElosEventVectorFromJsonObjectExtErrNewVectorSetup(void **state) {
    elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(state);
    return 0;
}

int elosTestElosEventVectorFromJsonObjectExtErrNewVectorTeardown(void **state) {
    elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(state);
    return 0;
}

void elosTestElosEventVectorFromJsonObjectExtErrNewVector(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventVectorNew failed");

    MOCK_FUNC_AFTER_CALL(elosEventVectorNew, 0);
    expect_not_value(elosEventVectorNew, eventVector, NULL);
    expect_value(elosEventVectorNew, elements, NUM_TEST_EVENTS);
    will_set_parameter(elosEventVectorNew, eventVector, NULL);
    will_return(elosEventVectorNew, SAFU_RESULT_FAILED);

    result = elosEventVectorFromJsonObject(testState->eventVecJobj, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
