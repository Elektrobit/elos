// SPDX-License-Identifier: MIT

#include "elosMessageEventPublish_utest.h"
#include "json-c/json.h"
#include "mock_LogAggregator.h"
#include "mock_event.h"
#include "mock_eventbuffer.h"
#include "mock_message_handler.h"
#include "safu/mock_log.h"

extern int elosMessageEventPublish(elosClientManagerConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishPublishFailedSetup(void **state) {
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

int elosTestElosMessageEventPublishPublishFailedTeardown(void **state) {
    elosUtestState_t *data = *state;
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventPublishPublishFailed(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    const char *msg =
        "{\"date\": [2000, 100], \"source\": \"Test\", \"severity\": 0, \"hardwareid\":\"0000\","
        "\"classification\": 0, \"messageCode\": 0, \"payload\": \"testevent\"}";
    const char *errstr = "Writing into the EventBuffer failed";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosEventProcessorPublish fails");

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    assert_non_null(data->msg);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    MOCK_FUNC_AFTER_CALL(elosEventBufferWrite, 0);
    expect_value(elosEventBufferWrite, eventBuffer, &data->conn->eventBuffer);
    expect_any(elosEventBufferWrite, event);
    will_return(elosEventBufferWrite, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorAdd, 0);
    expect_value(elosLogAggregatorAdd, logAggregator, data->conn->sharedData->logAggregator);
    expect_any(elosLogAggregatorAdd, event);
    will_return(elosLogAggregatorAdd, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, errstr);
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0)
    expect_string(elosMessageHandlerResponseCreate, errstr, errstr);
    will_return(elosMessageHandlerResponseCreate, data->response);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_value(elosMessageHandlerSendJson, jobj, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_FAILED);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_FAILED);
}
