// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_eventfiltermanager.h"
#include "mock_eventprocessor.h"
#include "mock_eventqueuemanager.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"
#include "safu/mock_vector.h"

int elosTestElosMessageEventSubscribeResponseNewArrayFailedSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    __real_safuVecCreate(&data->conn->data.eventFilterNodeIdVector, 1, sizeof(int));
    __real_safuVecCreate(&data->conn->data.eventQueueIdVector, 1, sizeof(int));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    data->response = __real_json_object_new_object();
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeResponseNewArrayFailedTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->jobj);
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

void elosTestElosMessageEventSubscribeResponseNewArrayFailed(void **state) {
    safuResultE_t result;
    int i;
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
    json_object *filter;
    json_object *arrayElement;
    const char *arrayElementString;
    elosEventFilterNodeId_t nodeId = 1;
    elosEventQueueId_t queueId = 1;

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "successfully simulate the message event subscription");

    data = (elosUtestState_t *)*state;
    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);
    data->jobj = __real_json_tokener_parse(msg);
    filter = __real_json_object_object_get(data->jobj, "filter");
    elosEventProcessor_t *eventProcessor = data->conn->sharedData->eventProcessor;

    assert_non_null(data->jobj);

    MOCK_FUNC_ALWAYS(json_tokener_parse);
    MOCK_FUNC_ALWAYS(json_object_put);
    MOCK_FUNC_ALWAYS(safuJsonAddNewArray);
    MOCK_FUNC_ALWAYS(safuJsonGetArray);
    MOCK_FUNC_ALWAYS(safuJsonGetString);
    MOCK_FUNC_ALWAYS(elosMessageHandlerResponseCreate);
    MOCK_FUNC_ALWAYS(elosMessageHandlerSendJson);
    MOCK_FUNC_ALWAYS(elosEventFilterManagerNodeRemove);
    MOCK_FUNC_ALWAYS(elosEventQueueManagerEntryDelete);
    MOCK_FUNC_ALWAYS(elosEventProcessorFilterNodeCreateWithQueue);
    MOCK_FUNC_ALWAYS(safuVecPush);
    MOCK_FUNC_ALWAYS(safuLogFuncF);

    expect_string(__wrap_json_tokener_parse, str, msg);
    will_return(__wrap_json_tokener_parse, data->jobj);

    expect_value(__wrap_safuJsonGetArray, jobj, data->jobj);
    expect_string(__wrap_safuJsonGetArray, name, "filter");
    expect_value(__wrap_safuJsonGetArray, idx, 0);
    expect_any(__wrap_safuJsonGetArray, len);
    will_set_parameter(__wrap_safuJsonGetArray, len, 3);
    will_return(__wrap_safuJsonGetArray, filter);

    for (i = 0; i < 3; i++) {
        arrayElement = __real_json_object_array_get_idx(filter, i);
        arrayElementString = __real_json_object_get_string(arrayElement);

        expect_value(__wrap_safuJsonGetString, jobj, filter);
        expect_value(__wrap_safuJsonGetString, name, NULL);
        expect_value(__wrap_safuJsonGetString, idx, i);
        expect_any(__wrap_safuJsonGetString, val);
        will_set_parameter(__wrap_safuJsonGetString, val, arrayElementString);
        will_return(__wrap_safuJsonGetString, 0);
    }

    expect_value(elosEventProcessorFilterNodeCreateWithQueue, eventProcessor, eventProcessor);
    expect_check(elosEventProcessorFilterNodeCreateWithQueue, filterStrings, elosAssertStringArraysEqual,
                 &arrayStringTestData);
    expect_value(elosEventProcessorFilterNodeCreateWithQueue, filterStringCount, 3);
    expect_any(elosEventProcessorFilterNodeCreateWithQueue, eventQueueId);
    expect_any(elosEventProcessorFilterNodeCreateWithQueue, eventFilterNodeId);
    will_set_parameter(elosEventProcessorFilterNodeCreateWithQueue, eventFilterNodeId, nodeId);
    will_set_parameter(elosEventProcessorFilterNodeCreateWithQueue, eventQueueId, queueId);
    will_return(elosEventProcessorFilterNodeCreateWithQueue, SAFU_RESULT_OK);

    expect_value(__wrap_safuVecPush, vec, &data->conn->data.eventFilterNodeIdVector);
    expect_memory(__wrap_safuVecPush, element, &nodeId, sizeof(int));
    will_return(__wrap_safuVecPush, 0);

    expect_value(__wrap_safuVecPush, vec, &data->conn->data.eventQueueIdVector);
    expect_memory(__wrap_safuVecPush, element, &queueId, sizeof(int));
    will_return(__wrap_safuVecPush, 0);

    expect_value(__wrap_json_object_put, obj, data->jobj);
    will_return(__wrap_json_object_put, 0);

    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, data->response);

    expect_value(__wrap_safuJsonAddNewArray, jobj, data->response);
    expect_string(__wrap_safuJsonAddNewArray, name, "eventQueueIds");
    will_return(__wrap_safuJsonAddNewArray, NULL);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "safuJsonAddNewArray failed");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_DEBUG);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "failed to create response eventqueue array");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    expect_value(elosEventFilterManagerNodeRemove, efm, &eventProcessor->eventFilterManager);
    expect_value(elosEventFilterManagerNodeRemove, efnId, nodeId);
    will_return(elosEventFilterManagerNodeRemove, SAFU_RESULT_OK);

    expect_value(elosEventQueueManagerEntryDelete, eqm, &eventProcessor->eventQueueManager);
    expect_value(elosEventQueueManagerEntryDelete, eqId, queueId);
    will_return(elosEventQueueManagerEntryDelete, SAFU_RESULT_OK);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    MOCK_FUNC_NEVER(json_tokener_parse);
    MOCK_FUNC_NEVER(json_object_put);
    MOCK_FUNC_NEVER(safuJsonAddNewArray);
    MOCK_FUNC_NEVER(safuJsonGetArray);
    MOCK_FUNC_NEVER(safuJsonGetString);
    MOCK_FUNC_NEVER(elosMessageHandlerResponseCreate);
    MOCK_FUNC_NEVER(elosMessageHandlerSendJson);
    MOCK_FUNC_NEVER(elosEventFilterManagerNodeRemove);
    MOCK_FUNC_NEVER(elosEventQueueManagerEntryDelete);
    MOCK_FUNC_NEVER(elosEventProcessorFilterNodeCreateWithQueue);
    MOCK_FUNC_NEVER(safuVecPush);
    MOCK_FUNC_NEVER(safuLogFuncF);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
