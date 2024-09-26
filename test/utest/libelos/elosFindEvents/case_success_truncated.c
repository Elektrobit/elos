// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <time.h>

#include "elosFindEvents_utest.h"
#include "mock_libelos_communication.h"

#define RESPONSE_CHUNKS_COUNT 3

int elosTestElosFindEventsSuccessTruncatedSetup(void **state) {
    static elosUteststateT_t testState = {0};
    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    testState.expectedJsonRequest = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);
    testState.expectedJsonResponse = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);

    assert_non_null_msg(testState.expectedJsonRequest, "safuAllocMem failed");
    assert_non_null_msg(testState.expectedJsonResponse, "safuAllocMem failed");

    return 0;
}

int elosTestElosFindEventsSuccessTruncatedTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosFindEventsUtestFreeSession(state);
    elosCleanupMockSendAndReceiveJsonMessage(testState);
    elosEventVectorDelete(testState->eventVector);
    return 0;
}

void elosTestElosFindEventsSuccessTruncated(void **state) {
    TEST("elosFindEvent");
    SHOULD("%s", "return SAFU_RESULT_OK on fetching a truncated list of events");

    elosUteststateT_t *testState = *state;

    testState->expectedJsonRequest[0] = json_tokener_parse(
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }");
    assert_non_null_msg(testState->expectedJsonRequest[0], "Failed to parse json request");

    testState->expectedJsonResponse[0] = json_tokener_parse(
        "{\
    \"eventArray\": [\
      {\"date\":[1,0], \"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[2,0],\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[3,0],\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ],\
    \"isTruncated\": true\
    }");
    assert_non_null_msg(testState->expectedJsonResponse[0], "Failed to parse json response");

    testState->expectedJsonRequest[1] = json_tokener_parse(
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [3,0]\
    }");
    assert_non_null_msg(testState->expectedJsonRequest[1], "Failed to parse json request");

    testState->expectedJsonResponse[1] = json_tokener_parse(
        "{\
    \"eventArray\": [\
      {\"date\":[4,0], \"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[5,0],\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[6,0],\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ],\
    \"isTruncated\": true\
    }");
    assert_non_null_msg(testState->expectedJsonResponse[1], "Failed to parse json response");

    testState->expectedJsonRequest[2] = json_tokener_parse(
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [6,0]\
    }");
    assert_non_null_msg(testState->expectedJsonRequest[2], "Failed to parse json request");

    testState->expectedJsonResponse[2] = json_tokener_parse(
        "{\
    \"eventArray\": [\
      {\"date\":[7,0], \"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[8,0],\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"date\":[9,0],\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ],\
    \"isTruncated\": false\
    }");
    assert_non_null_msg(testState->expectedJsonResponse[2], "Failed to parse json response");
    elosMockSendAndReceiveJsonMessage(testState, RESPONSE_CHUNKS_COUNT, SAFU_RESULT_OK);

    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};

    safuResultE_t result = elosFindEvents(mockSession, filterRule, &newest, &oldest, &testState->eventVector);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(testState->eventVector);
    assert_int_equal(testState->eventVector->elementCount, 9);
    elosEvent_t *event;
    event = safuVecGet(testState->eventVector, 0);
    assert_int_equal(event->messageCode, 1001);
    assert_string_equal(event->payload, "Hugo hat Husten");
    struct timespec lastTimestamp = {.tv_sec = 1, .tv_nsec = 0};
    assert_memory_equal(&event->date, &lastTimestamp, sizeof(struct timespec));

    event = safuVecGetLast(testState->eventVector);
    assert_int_equal(event->messageCode, 1003);
    assert_string_equal(event->payload, "Hugo hat Husten");
    lastTimestamp = (struct timespec){.tv_sec = 9, .tv_nsec = 0};
    assert_memory_equal(&event->date, &lastTimestamp, sizeof(struct timespec));
}
