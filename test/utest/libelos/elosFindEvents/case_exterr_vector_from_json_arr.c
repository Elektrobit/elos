// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>

#include "elosFindEvents_utest.h"
#include "mock_event_vector.h"
#include "mock_libelos_communication.h"

#define RESPONSE_CHUNKS_COUNT 1

int elosTestElosFindEventsExtErrVectorFromJsonArrSetup(void **state) {
    static elosUteststateT_t testState = {0};
    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    testState.expectedJsonRequest = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);
    testState.expectedJsonResponse = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);

    assert_non_null_msg(testState.expectedJsonRequest, "safuAllocMem failed");
    assert_non_null_msg(testState.expectedJsonResponse, "safuAllocMem failed");

    return 0;
}

int elosTestElosFindEventsExtErrVectorFromJsonArrTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosFindEventsUtestFreeSession(state);
    elosCleanupMockSendAndReceiveJsonMessage(testState);
    return 0;
}

void elosTestElosFindEventsExtErrVectorFromJsonArr(void **state) {
    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventVectorFromJsonArray returns an empty event vector");

    elosUteststateT_t *testState = *state;
    const char *findEventsRequest =
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }";
    testState->expectedJsonRequest[0] = json_tokener_parse(findEventsRequest);
    assert_non_null_msg(testState->expectedJsonRequest[0], "Failed to parse json request");

    const char *findEventsResponse =
        "{\
    \"eventArray\": [\
      {\"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ],\
    \"isTruncated\": false\
    }";
    testState->expectedJsonResponse[0] = json_tokener_parse(findEventsResponse);
    assert_non_null_msg(testState->expectedJsonResponse[0], "Failed to parse json response");

    elosMockSendAndReceiveJsonMessage(testState, RESPONSE_CHUNKS_COUNT, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventVectorFromJsonArray, 0);
    expect_not_value(elosEventVectorFromJsonArray, jEventArray, NULL);
    expect_not_value(elosEventVectorFromJsonArray, eventVector, NULL);
    will_set_parameter(elosEventVectorFromJsonArray, eventVector, NULL);
    will_return(elosEventVectorFromJsonArray, SAFU_RESULT_FAILED);

    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    elosEventVector_t *vector = NULL;
    safuResultE_t result = elosFindEvents(mockSession, filterRule, &newest, &oldest, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
