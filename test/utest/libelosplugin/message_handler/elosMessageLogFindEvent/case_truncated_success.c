// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elosMessageLogFindEvent_utest.h"
#include "mock_message_handler.h"
extern int elosMessageLogFindEvent(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

static safuResultE_t _mockElosPluginFindEvents(struct elosPluginControl *pluginController, const char *const rule,
                                               UNUSED const struct timespec *newest,
                                               UNUSED const struct timespec *oldest, safuVec_t *events) {
    safuResultE_t result;
    check_expected_ptr(pluginController);
    check_expected_ptr(rule);
    check_expected_ptr(newest);
    check_expected_ptr(oldest);
    check_expected_ptr(events);

    result = mock_type(safuResultE_t);
    return result;
}

static void _generateRandomPayload(char *payload, size_t payloadSize) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < payloadSize; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        payload[i] = charset[key];
    }
    payload[payloadSize] = '\0';
}

int elosTestElosMessageLogFindEventTruncatedSuccessSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(data->conn->sharedData);
    data->conn->sharedData->plugin = safuAllocMem(NULL, sizeof(elosPlugin_t));
    assert_non_null(data->conn->sharedData->plugin);

    data->conn->sharedData->plugin->findEvents = _mockElosPluginFindEvents;
    *state = data;
    return 0;
}

int elosTestElosMessageLogFindEventTruncatedSuccessTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->expectedJsonResponse);
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    free(data->conn);
    free(data->msg);
    free(data);
    return 0;
}

void elosTestElosMessageLogFindEventTruncatedSuccess(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    size_t size = UINT16_MAX - sizeof(elosMessage_t);
    char hugePayload[size + 1];
    srand(time(NULL));

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_OK on fetching successfully an truncated event list");

    const char findEventsRequest[] =
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }";
    data->msg = safuAllocMem(NULL, sizeof(*(data->msg)) + sizeof(findEventsRequest));
    assert_non_null_msg(data->msg, "safuAllocMem failed");
    strcpy(data->msg->json, findEventsRequest);
    data->msg->length = 1;

    char *hugeEventPrefix =
        "{\
    \"error\":null,\
    \"isTruncated\": true,\
    \"eventArray\": [\
      {\"source\": { },\"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"source\": { },\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"source\": { },\"messageCode\": 1003,\"payload\": \"";

    _generateRandomPayload(hugePayload, size);

    char *hugeEventSuffix =
        "\"}\
    ]\
    }";

    char hugeEvent[strlen(hugeEventPrefix) + sizeof(hugePayload) + strlen(hugeEventSuffix)];

    int written = snprintf(hugeEvent, sizeof(hugeEvent), "%s%s%s", hugeEventPrefix, hugePayload, hugeEventSuffix);

    assert_true((written > 0));

    json_object *hugeJsonResponse = json_tokener_parse(hugeEvent);

    data->expectedJsonResponse = json_tokener_parse(
        "{\
    \"error\":null,\
    \"isTruncated\": true,\
    \"eventArray\": [\
      {\"source\": { },\"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"source\": { },\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"}\
    ]\
    }");

    expect_any(_mockElosPluginFindEvents, pluginController);
    expect_any(_mockElosPluginFindEvents, newest);
    expect_any(_mockElosPluginFindEvents, oldest);
    expect_string(_mockElosPluginFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_check(_mockElosPluginFindEvents, events, elosMessageLogFindEventInjectEventList, hugeJsonResponse);
    will_return(_mockElosPluginFindEvents, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);

    expect_not_value(elosMessageHandlerSendJson, conn, NULL);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT);
    expect_check(elosMessageHandlerSendJson, jobj, elosMessageLogFindEventVerifyResponse, data->expectedJsonResponse);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageLogFindEvent(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
    json_object_put(hugeJsonResponse);
}
