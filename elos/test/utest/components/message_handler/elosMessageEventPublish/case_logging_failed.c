// SPDX-License-Identifier: MIT

#include "elosMessageEventPublish_utest.h"
#include "json-c/json.h"
#include "mock_LogAggregator.h"
#include "mock_event.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"
#include "safu/mock_log.h"

extern int elosMessageEventPublish(elosClientManagerConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishLoggingFailedSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    assert_non_null(data->conn);
    data->conn->isTrusted = true;
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    assert_non_null(data->conn->sharedData);
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    assert_non_null(data->conn->sharedData->eventProcessor);
    data->response = json_object_new_object();
    assert_non_null(data->response);
    *state = data;
    return 0;
}

int elosTestElosMessageEventPublishLoggingFailedTeardown(void **state) {
    elosUtestState_t *data = *state;
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventPublishLoggingFailed(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    const char *msg =
        "{\"date\": [2000, 100], \"Source\": \"Test\", \"severity\": 0, \"hardwareid\":\"0000\", "
        "\"classification\": 0, \"messageCode\": 200, \"payload\": \"testevent\"}";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosLogAggregatorAdd fails");

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    assert_non_null(data->msg);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    MOCK_FUNC_AFTER_CALL(elosEventProcessorPublish, 0);
    expect_value(elosEventProcessorPublish, eventProcessor, data->conn->sharedData->eventProcessor);
    expect_any(elosEventProcessorPublish, event);
    will_return(elosEventProcessorPublish, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorAdd, 0);
    expect_value(elosLogAggregatorAdd, logAggregator, data->conn->sharedData->logAggregator);
    expect_any(elosLogAggregatorAdd, event);
    will_return(elosLogAggregatorAdd, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0)
    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, data->response);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_value(elosMessageHandlerSendJson, jobj, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_FAILED);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_FAILED);
}
