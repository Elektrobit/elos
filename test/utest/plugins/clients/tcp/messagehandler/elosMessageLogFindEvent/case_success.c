// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <elos/common/message.h>
#include <elos/event/event_types.h>
#include <elos/event/event_vector.h>
#include <elos/libelosplugin/types.h>
#include <json_object.h>
#include <json_tokener.h>
#include <json_types.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <string.h>

#include "elosMessageLogFindEvent_utest.h"
#include "mock_message_handler.h"

extern int elosMessageLogFindEvent(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

static safuResultE_t _mockElosPluginFindEvents(struct elosPluginControl *pluginController, const char *const rule,
                                               safuVec_t *events) {
    safuResultE_t result;
    check_expected_ptr(pluginController);
    check_expected_ptr(rule);
    check_expected_ptr(events);

    result = mock_type(safuResultE_t);
    return result;
}

int elosTestElosMessageLogFindEventSuccessSetup(void **state) {
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

int elosTestElosMessageLogFindEventSuccessTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->expectedJsonResponse);
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

int _verifyResponse(const LargestIntegralType actual, const LargestIntegralType expected) {
    json_object *actualJsonResponse = (json_object *)actual;
    json_object *expectedJsonResponse = (json_object *)expected;
    int isEqual = json_object_equal(actualJsonResponse, expectedJsonResponse);
    if (isEqual == 0) {
        print_error("Request is not as expected: actual '%s' !=  expected '%s'\n",
                    json_object_to_json_string(actualJsonResponse), json_object_to_json_string(expectedJsonResponse));
    }
    return isEqual;
}

int _injectEventList(const LargestIntegralType eventVector, const LargestIntegralType expected) {
    elosEventVector_t *events = (elosEventVector_t *)eventVector;
    json_object *expectedJsonResponse = (json_object *)expected;

    elosEventVector_t *injectedEvents = NULL;
    size_t eventCount = 0;
    json_object *jEventArray = safuJsonGetArray(expectedJsonResponse, "eventArray", 0, &eventCount);
    elosEventVectorFromJsonArray(jEventArray, &injectedEvents);

    for (size_t i = 0; i < eventCount; i++) {
        safuVecPush(events, safuVecGet(injectedEvents, i));
    }

    safuVecFree(injectedEvents);
    free(injectedEvents);

    return 1;
}

void elosTestElosMessageLogFindEventSuccess(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_OK on fetching successful an untruncated event list");

    const char findEventsRequest[] =
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }";
    data->msg = safuAllocMem(NULL, sizeof(*(data->msg)) + sizeof(findEventsRequest));
    assert_non_null_msg(data->msg, "safuAllocMem failed");
    strcpy(data->msg->json, findEventsRequest);

    data->expectedJsonResponse = json_tokener_parse(
        "{\
    \"error\":null,\
    \"eventArray\": [\
      {\"messageCode\": 1001,\"payload\": \"Hugo hat Husten\"},\
      {\"messageCode\": 1002,\"payload\": \"Hugo hat Husten\"},\
      {\"messageCode\": 1003,\"payload\": \"Hugo hat Husten\"}\
    ],\
    \"isTruncated\": false\
    }");

    expect_not_value(_mockElosPluginFindEvents, pluginController, NULL);
    expect_string(_mockElosPluginFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_check(_mockElosPluginFindEvents, events, _injectEventList, data->expectedJsonResponse);
    will_return(_mockElosPluginFindEvents, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);

    expect_not_value(elosMessageHandlerSendJson, conn, NULL);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT);
    expect_check(elosMessageHandlerSendJson, jobj, _verifyResponse, data->expectedJsonResponse);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageLogFindEvent(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
}
