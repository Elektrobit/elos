// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"
#include "mock_event.h"

int elosTestElosEventVectorFromJsonObjectExtErrEventFromJsonObjectSetup(void **state) {
    elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(state);
    return 0;
}

int elosTestElosEventVectorFromJsonObjectExtErrEventFromJsonObjectTeardown(void **state) {
    elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(state);
    return 0;
}

void elosTestElosEventVectorFromJsonObjectExtErrEventFromJsonObject(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventFromJsonObject failed");

    MOCK_FUNC_AFTER_CALL(elosEventFromJsonObject, 0);
    expect_not_value(elosEventFromJsonObject, to, NULL);
    expect_not_value(elosEventFromJsonObject, from, NULL);
    will_return(elosEventFromJsonObject, SAFU_RESULT_FAILED);

    result = elosEventVectorFromJsonObject(testState->eventVecJobj, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
