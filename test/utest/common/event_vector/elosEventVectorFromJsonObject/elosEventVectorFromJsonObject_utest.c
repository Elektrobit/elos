// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorFromJsonObjectUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorFromJsonObjectErrEventVectorJobj),
                                       TEST_CASE(elosTestElosEventVectorFromJsonObjectErrEventVector),
                                       TEST_CASE(elosTestElosEventVectorFromJsonObjectExtErrGetArray),
                                       TEST_CASE(elosTestElosEventVectorFromJsonObjectExtErrNewVector),
                                       TEST_CASE(elosTestElosEventVectorFromJsonObjectExtErrEventFromJsonObject),
                                       TEST_CASE(elosTestElosEventVectorFromJsonObjectSuccess)};

    return RUN_TEST_SUITE(tests, elosEventVectorFromJsonObjectUtest);
}

void elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVector_t *eventVector = NULL;

    safuResultE_t result = elosEventVectorNew(&eventVector, NUM_TEST_EVENTS);
    assert_int_equal(result, SAFU_RESULT_OK);

    const elosEvent_t testEvent = {.messageCode = 42};

    safuVecPush(eventVector, &testEvent);
    testState->expectedEventVector = eventVector;

    result = elosEventVectorToJsonObject(eventVector, &testState->eventVecJobj);
    assert_int_equal(result, SAFU_RESULT_OK);
}

void elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVectorDelete(testState->expectedEventVector);
    json_object_put(testState->eventVecJobj);
}

static int elosEventVectorFromJsonObjectUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorFromJsonObjectUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
