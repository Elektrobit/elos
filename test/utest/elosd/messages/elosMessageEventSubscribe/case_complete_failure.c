// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_message_handler.h"
#include "safu/mock_log.h"

int elosTestElosMessageEventSubscribeCompleteFailureSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->response = json_object_new_object();
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeCompleteFailureTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeCompleteFailure(void **state) {
    safuResultE_t result;
    elosUtestState_t *data;
    const char msg[] = "[\"not json\",]";

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "fail since the given json text was invalid and returned no json object upon parsing");

    data = (elosUtestState_t *)*state;
    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    // enable mocking anyway to ensure it catches fails.
    MOCK_FUNC_ALWAYS(json_tokener_parse);
    MOCK_FUNC_ALWAYS(json_object_object_get);
    MOCK_FUNC_ALWAYS(json_object_get_type);
    MOCK_FUNC_ALWAYS(json_object_array_length);
    MOCK_FUNC_ALWAYS(json_object_array_get_idx);
    MOCK_FUNC_ALWAYS(json_object_get_string);
    MOCK_FUNC_ALWAYS(json_object_put);
    MOCK_FUNC_ALWAYS(elosMessageHandlerResponseCreate);
    MOCK_FUNC_ALWAYS(elosMessageHandlerSendJson);

    expect_string(__wrap_json_tokener_parse, str, msg);
    will_return(__wrap_json_tokener_parse, NULL);

    expect_string(elosMessageHandlerResponseCreate, errstr, "failed to parse json");
    will_return(elosMessageHandlerResponseCreate, NULL);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    MOCK_FUNC_NEVER(json_tokener_parse);
    MOCK_FUNC_NEVER(json_object_object_get);
    MOCK_FUNC_NEVER(json_object_get_type);
    MOCK_FUNC_NEVER(json_object_array_length);
    MOCK_FUNC_NEVER(json_object_array_get_idx);
    MOCK_FUNC_NEVER(json_object_get_string);
    MOCK_FUNC_NEVER(json_object_put);
    MOCK_FUNC_NEVER(elosMessageHandlerResponseCreate);
    MOCK_FUNC_NEVER(elosMessageHandlerSendJson);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
