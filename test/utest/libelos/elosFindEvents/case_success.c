// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/common.h>

#include "elosFindEvents_utest.h"
#include "mock_libelos_communication.h"

#define RESPONSE_CHUNKS_COUNT 1

int elosTestElosFindEventsSuccessSetup(void **state) {
    static elosUteststateT_t testState = {0};
    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    testState.expectedJsonRequest = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);
    testState.expectedJsonResponse = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);

    assert_non_null_msg(testState.expectedJsonRequest, "safuAllocMem failed");
    assert_non_null_msg(testState.expectedJsonResponse, "safuAllocMem failed");

    return 0;
}

int elosTestElosFindEventsSuccessTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVectorDelete(testState->eventVector);
    elosFindEventsUtestFreeSession(state);
    elosCleanupMockSendAndReceiveJsonMessage(testState);
    return 0;
}

void elosTestElosFindEventsSuccess(void **state) {
    TEST("elosFindEvent");
    SHOULD("%s", "return SAFU_RESULT_OK on fetching an untruncated list of events");

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

    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};

    safuResultE_t result = elosFindEvents(mockSession, filterRule, &newest, &oldest, &testState->eventVector);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(testState->eventVector);
    assert_int_equal(testState->eventVector->elementCount, 3);

    elosEvent_t *event = NULL;
    event = safuVecGet(testState->eventVector, 0);
    assert_int_equal(event->messageCode, 1001);
    assert_string_equal(event->payload, "Hugo hat Husten");

    event = safuVecGetLast(testState->eventVector);
    assert_int_equal(event->messageCode, 1003);
    assert_string_equal(event->payload, "Hugo hat Husten");
}
