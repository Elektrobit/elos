// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_message_handler.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventSubscribeNoJsonSetup(void **state) {
    MOCK_FUNC_NEVER(safuAllocMem);
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeNoJsonTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeNoJson(void **state) {
    safuResultE_t result;
    elosUtestState_t *data;
    const char msg[] = "}\"not json\",[";

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "fail since the given json text was invalid and returned no json object upon parsing");

    data = (elosUtestState_t *)*state;
    assert_non_null(data);

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    data->response = elosMessageHandlerResponseCreate("failed to parse json");

    assert_non_null(data->response);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson);

    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE);
    expect_check(elosMessageHandlerSendJson, jobj, elosCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, 0);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
