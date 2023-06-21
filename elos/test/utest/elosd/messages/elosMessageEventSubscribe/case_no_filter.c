// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_message_handler.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventSubscribeNoFilterSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeNoFilterTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeNoFilter(void **state) {
    safuResultE_t result;
    elosUtestState_t *data;
    const char msg[] = "[\"not json\"]";

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "fail since the given json string doesnt contain an key 'filter'");

    data = (elosUtestState_t *)*state;
    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    data->response = elosMessageHandlerResponseCreate("failed to extract filter array");

    assert_non_null(data->response);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson);

    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE);
    expect_check(elosMessageHandlerSendJson, jobj, elosCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, -1);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
