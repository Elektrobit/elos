// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"

#include "safu/common.h"

#define NUM_TEST_EVENTS 1

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorToJsonObjectUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorToJsonObjectErrEventVector),
                                       TEST_CASE(elosTestElosEventVectorToJsonObjectErrEventVectorJobj),
                                       TEST_CASE(elosTestElosEventVectorToJsonObjectExtErrNewJobj),
                                       TEST_CASE(elosTestElosEventVectorToJsonObjectExtErrAddNewArray),
                                       TEST_CASE(elosTestElosEventVectorToJsonObjectSuccess)};

    return RUN_TEST_SUITE(tests, elosEventVectorToJsonObjectUtest);
}

void elosEventVectorToJsonObjectUtestCreateEventVector(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = elosEventVectorNew(&testState->eventVector, NUM_TEST_EVENTS);
    assert_int_equal(result, SAFU_RESULT_OK);

    elosEventSource_t testEventSource = {
        .appName = "testApp",
        .fileName = "testFile",
        .pid = 123,
    };

    const elosEvent_t testEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = testEventSource,
        .severity = 0,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = 32,
        .payload = "payload",
    };

    elosEvent_t testEventPush = {0};
    result = elosEventDeepCopy(&testEventPush, &testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
    safuVecPush(testState->eventVector, &testEventPush);  // copies event and free's its members
}

void elosEventVectorToJsonObjectUtestDeleteEventVector(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVectorDelete(testState->eventVector);
}

static int elosEventVectorToJsonObjectUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorToJsonObjectUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
