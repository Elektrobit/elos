// SPDX-License-Identifier: MIT

#include "elosFindEvents_utest.h"
#include "mock_libelos_communication.h"

#define RESPONSE_CHUNKS_COUNT 1

int elosTestElosFindEventsExtErrCommunicationSetup(void **state) {
    static elosUteststateT_t testState = {0};
    *state = &testState;
    elosFindEventsUtestCreateSession(state);
    testState.expectedJsonRequest = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);
    testState.expectedJsonResponse = safuAllocMem(NULL, sizeof(json_object *) * RESPONSE_CHUNKS_COUNT);

    assert_non_null_msg(testState.expectedJsonRequest, "safuAllocMem failed");
    assert_non_null_msg(testState.expectedJsonResponse, "safuAllocMem failed");
    return 0;
}

int elosTestElosFindEventsExtErrCommunicationTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosFindEventsUtestFreeSession(state);
    elosCleanupMockSendAndReceiveJsonMessage(testState);
    return 0;
}

void elosTestElosFindEventsExtErrCommunication(void **state) {
    TEST("elosFindEvents");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosSendAndReceiveJsonMessage failed");

    elosUteststateT_t *testState = *state;

    const char *findEventsRequest =
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }";
    testState->expectedJsonRequest[0] = json_tokener_parse(findEventsRequest);
    assert_non_null_msg(testState->expectedJsonRequest[0], "Failed to parse json request");
    testState->expectedJsonResponse[0] = NULL;
    elosMockSendAndReceiveJsonMessage(testState, RESPONSE_CHUNKS_COUNT, SAFU_RESULT_FAILED);

    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    struct timespec newest = {.tv_sec = 0, .tv_nsec = 0};
    struct timespec oldest = {.tv_sec = 0, .tv_nsec = 0};
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    result = elosFindEvents(mockSession, filterRule, &newest, &oldest, &vector);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
