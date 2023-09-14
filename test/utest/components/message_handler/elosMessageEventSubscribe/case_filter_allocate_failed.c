// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"
#include "safu/mock_vector.h"

int elosTestElosMessageEventSubscribeFilterAllocateFailedSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeFilterAllocateFailedTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->response);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data->msg);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeFilterAllocateFailed(void **state) {
    safuResultE_t result;
    const char *msg =
        "{\"filter\":["
        "\"event.source.appName,'myApp',eq\","
        "\"event.source.appName,'linux',eq\","
        "\"event.source.appName,'openssh',eq\""
        "]}";
    elosUtestState_t *data;

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "fail as allocation for filter array failed");

    data = (elosUtestState_t *)*state;
    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    data->response = elosMessageHandlerResponseCreate("failed to allocate memory for filter array");

    assert_non_null(data->response);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson);
    MOCK_FUNC_ENABLE(safuAllocMem);

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, 3 * sizeof(char *));
    will_return(__wrap_safuAllocMem, NULL);

    expect_value(elosMessageHandlerSendJson, conn, data->conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE);
    expect_check(elosMessageHandlerSendJson, jobj, elosCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, 0);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
