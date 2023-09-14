// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/mock_safu.h"
#include "safu/mock_vector.h"

int elosTestElosMessageEventSubscribeSuccessSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    __real_safuVecCreate(&data->conn->data.eventFilterNodeIdVector, 1, sizeof(int));
    __real_safuVecCreate(&data->conn->data.eventQueueIdVector, 1, sizeof(int));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    data->response = elosMessageHandlerResponseCreate(NULL);
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeSuccessTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->response);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    __real_safuVecFree(&data->conn->data.eventFilterNodeIdVector);
    __real_safuVecFree(&data->conn->data.eventQueueIdVector);
    free(data->conn);
    free(data->msg);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeSuccess(void **state) {
    safuResultE_t result;
    const char *msg =
        "{\"filter\":["
        "\"event.source.appName,'myApp',eq\","
        "\"event.source.appName,'linux',eq\","
        "\"event.source.appName,'openssh',eq\""
        "]}";
    const char *filterStrings[] = {"event.source.appName,'myApp',eq", "event.source.appName,'linux',eq",
                                   "event.source.appName,'openssh',eq"};
    elosArrayStringTestData_t arrayStringTestData = {filterStrings, 3};
    elosUtestState_t *data;
    elosEventFilterNodeId_t nodeId = 1;
    elosEventQueueId_t queueId = 1;

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "successfully simulate the message event subscription");

    data = (elosUtestState_t *)*state;
    assert_non_null(data);

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    assert_non_null(data->conn);
    assert_non_null(data->conn->sharedData);
    assert_non_null(data->conn->sharedData->eventProcessor);

    elosEventProcessor_t *eventProcessor = data->conn->sharedData->eventProcessor;

    assert_non_null(data->response);
    json_object *queIdArray = safuJsonAddNewArray(data->response, "eventQueueIds");
    safuJsonAddNewInt64(queIdArray, NULL, queueId);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson);
    MOCK_FUNC_ENABLE(elosEventProcessorFilterNodeCreateWithQueue);

    expect_value(elosEventProcessorFilterNodeCreateWithQueue, eventProcessor, eventProcessor);
    expect_check(elosEventProcessorFilterNodeCreateWithQueue, filterStrings, elosAssertStringArraysEqual,
                 &arrayStringTestData);
    expect_value(elosEventProcessorFilterNodeCreateWithQueue, filterStringCount, 3);
    expect_any(elosEventProcessorFilterNodeCreateWithQueue, eventQueueId);
    expect_any(elosEventProcessorFilterNodeCreateWithQueue, eventFilterNodeId);
    will_set_parameter(elosEventProcessorFilterNodeCreateWithQueue, eventFilterNodeId, nodeId);
    will_set_parameter(elosEventProcessorFilterNodeCreateWithQueue, eventQueueId, queueId);
    will_return(elosEventProcessorFilterNodeCreateWithQueue, SAFU_RESULT_OK);

    expect_value(elosMessageHandlerSendJson, conn, data->conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE);
    expect_check(elosMessageHandlerSendJson, jobj, elosCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, 0);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    assert_int_equal(result, SAFU_RESULT_OK);
}
