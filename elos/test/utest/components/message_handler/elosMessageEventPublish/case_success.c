// SPDX-License-Identifier: MIT

#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter.h"
#include "elosMessageEventPublish_utest.h"
#include "json-c/json.h"
#include "mock_LogAggregator.h"
#include "mock_event.h"
#include "mock_eventbuffer.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"

extern int elosMessageEventPublish(elosClientManagerConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishSuccessSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    assert_non_null(data->conn);
    data->conn->isTrusted = true;
    data->conn->blacklist = (elosEventFilter_t){0};
    assert_int_equal(data->conn->blacklist.memorySize, 0);
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    assert_non_null(data->conn->sharedData);
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    assert_non_null(data->conn->sharedData->eventProcessor);

    data->response = elosMessageHandlerResponseCreate(NULL);
    assert_non_null(data->response);

    const char *msg =
        "{\"topicID\": 0, \"topic\": \"test\", \"date\": [2000, 100], \"source\": \"Test\", \"severity\": 0, "
        "\"hardwareid\":\"0000\", \"classification\": 0, \"messageCode\": 0, \"payload\": \"testevent\"}";
    data->event = NULL;

    safuResultE_t tmpRes = elosEventNew(&data->event);
    assert_int_equal(tmpRes, SAFU_RESULT_OK);
    tmpRes = elosEventDeserialize(data->event, msg);
    assert_int_equal(tmpRes, SAFU_RESULT_OK);

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    assert_non_null(data->msg);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    *state = data;
    return 0;
}

int elosTestElosMessageEventPublishSuccessTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    elosEventDelete(data->event);
    free(data);
    return 0;
}

static int _check_json_object(const long unsigned int parameter, const long unsigned int compare) {
    json_object *jobjParam = (json_object *)parameter;
    json_object *jobjCmp = (json_object *)compare;
    if (json_object_equal(jobjParam, jobjCmp)) {
        return 1;
    }
    print_error("%s != %s\n", json_object_to_json_string(jobjParam), json_object_to_json_string(jobjCmp));
    return 0;
}

static int _check_event(const long unsigned int parameter, const long unsigned int compare) {
    int result = 1;
    json_object *eventParam = json_object_new_object();
    json_object *eventCmp = json_object_new_object();
    safuResultE_t res = elosEventToJsonObject(eventParam, (elosEvent_t *)parameter);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    res = elosEventToJsonObject(eventCmp, (elosEvent_t *)compare);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    if (result == 1) {
        result = _check_json_object((const long unsigned int)eventParam, (const long unsigned int)eventCmp);
    }

    json_object_put(eventParam);
    json_object_put(eventCmp);

    return result;
}

void elosTestElosMessageEventPublishSuccess(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    TEST("elosMessageEventPublish");
    SHOULD("%s", "publish event successfully");

    MOCK_FUNC_AFTER_CALL(elosEventBufferWrite, 0);
    expect_value(elosEventBufferWrite, eventBuffer, &data->conn->eventBuffer);
    expect_check(elosEventBufferWrite, event, _check_event, data->event);
    will_return(elosEventBufferWrite, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorAdd, 0);
    expect_value(elosLogAggregatorAdd, logAggregator, data->conn->sharedData->logAggregator);
    expect_check(elosLogAggregatorAdd, event, _check_event, data->event);
    will_return(elosLogAggregatorAdd, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_check(elosMessageHandlerSendJson, jobj, _check_json_object, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
}
