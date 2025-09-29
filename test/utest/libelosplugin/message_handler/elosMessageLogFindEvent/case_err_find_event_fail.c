// SPDX-License-Identifier: MIT

#include <json-c/json_types.h>
#include <safu/result.h>

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

int elosTestElosMessageLogFindEventErrorSetup(void **state) {
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

int elosTestElosMessageLogFindEventErrorTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->expectedJsonResponse);
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    free(data->conn);
    free(data->msg);
    free(data);
    return 0;
}

void elosTestElosMessageLogFindEventError(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "send error response upon failing to fetch request filter");

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

    json_object *jsonResponse = json_tokener_parse(
        "{\
    \"error\":null,\
    \"isTruncated\": false,\
    \"eventArray\": [\
      {\"source\": { },\"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"source\": { },\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"source\": { },\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ]\
    }");

    data->expectedJsonResponse = json_tokener_parse("{\"error\": \"Failed to find events\"}");

    expect_any(_mockElosPluginFindEvents, pluginController);
    expect_any(_mockElosPluginFindEvents, newest);
    expect_any(_mockElosPluginFindEvents, oldest);
    expect_string(_mockElosPluginFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_check(_mockElosPluginFindEvents, events, elosMessageLogFindEventInjectEventList, jsonResponse);
    will_return(_mockElosPluginFindEvents, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);

    expect_not_value(elosMessageHandlerSendJson, conn, NULL);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT);
    expect_check(elosMessageHandlerSendJson, jobj, elosMessageLogFindEventVerifyResponse, data->expectedJsonResponse);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageLogFindEvent(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
    json_object_put(jsonResponse);
}
