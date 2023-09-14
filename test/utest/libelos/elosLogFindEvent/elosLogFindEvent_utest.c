// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "elosLogFindEvent_utest.h"

#include <stdio.h>

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosLogFindEventErrSession),
                                       TEST_CASE(elosTestElosLogFindEventErrFilterRule),
                                       TEST_CASE(elosTestElosLogFindEventErrVector),
                                       TEST_CASE(elosTestElosLogFindEventExtErrVerifySession),
                                       TEST_CASE(elosTestElosLogFindEventExtErrNewObject),
                                       TEST_CASE(elosTestElosLogFindEventExtErrAddNewString),
                                       TEST_CASE(elosTestElosLogFindEventExtErrCommunication),
                                       TEST_CASE(elosTestElosLogFindEventExtErrVectorFromJsonArr),
                                       TEST_CASE(elosTestElosLogFindEventSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void elosLogFindEventUtestCreateSession(void **state) {
    elosUteststateT_t *testState = *state;
    testState->session = safuAllocMem(NULL, sizeof(elosSession_t));
    assert_non_null(testState->session);
}

void elosLogFindEventUtestFreeSession(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->session);
}

void elosLogFindEventUtestCreateMessage(elosMessage_t **message, size_t *messageLength, uint8_t messageId,
                                        const char *payload, size_t payloadSize) {
    elosMessage_t *newMessage = NULL;
    *messageLength = sizeof(elosMessage_t) + payloadSize;

    newMessage = safuAllocMem(NULL, *messageLength);
    assert_non_null(newMessage);

    memset(newMessage, 0, *messageLength);
    newMessage->version = ELOS_PROTOCOL_VERSION;
    newMessage->message = messageId;
    newMessage->length = payloadSize;
    memcpy(newMessage->json, payload, payloadSize);

    *message = newMessage;
}

void elosLogFindEventUtestDeleteMessage(elosMessage_t *message) {
    free(message);
}

void elosLogFindEventUtestCreateResponse(void **state) {
    elosUteststateT_t *testState = *state;

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

    const char eventVecStringStart[] = "{\"eventArray\":[";
    char *eventVecStringMid = NULL;
    safuResultE_t retval = elosEventSerialize(&eventVecStringMid, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    const char eventVecStringEnd[] = "]}";

    size_t expLen = strlen(eventVecStringStart) + strlen(eventVecStringMid) + strlen(eventVecStringEnd);

    char *eventVecString = NULL;
    int recLen = asprintf(&eventVecString, "%s%s%s", eventVecStringStart, eventVecStringMid, eventVecStringEnd);
    assert_int_equal(expLen, recLen);

    testState->responsePayload = eventVecString;
    testState->responsePayloadLength = expLen;

    elosLogFindEventUtestCreateMessage(&testState->response, &testState->responseLength,
                                       ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT, eventVecString, expLen);

    free(eventVecStringMid);
}

void elosLogFindEventUtestDeleteResponse(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->responsePayload);
    elosLogFindEventUtestDeleteMessage(testState->response);
}

void elosMockSendAndReceiveJsonMessage(elosUteststateT_t *test, char const *responseStr, safuResultE_t result) {
    json_object *responseObj = NULL;

    if (responseStr != NULL) {
        // elosLogFindEvent frees the jsonObject it created, so we have to give it one where it is able to do so
        responseObj = json_tokener_parse(responseStr);
        assert_non_null(responseObj);
    }

    MOCK_FUNC_AFTER_CALL(elosSendAndReceiveJsonMessage, 0);
    expect_value(elosSendAndReceiveJsonMessage, session, test->session);
    expect_value(elosSendAndReceiveJsonMessage, messageId, ELOS_MESSAGE_LOG_FIND_EVENT);
    expect_not_value(elosSendAndReceiveJsonMessage, sendJsonObject, NULL);
    expect_not_value(elosSendAndReceiveJsonMessage, receiveJsonObject, NULL);
    will_set_parameter(elosSendAndReceiveJsonMessage, receiveJsonObject, responseObj);
    will_return(elosSendAndReceiveJsonMessage, result);
}
