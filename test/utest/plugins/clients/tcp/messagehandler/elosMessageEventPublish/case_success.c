// SPDX-License-Identifier: MIT

#include <elos/event/event_types.h>
#include <elos/libelosplugin/types.h>
#include <safu/common.h>

#include "elos/event/event.h"
#include "elosMessageEventPublish_utest.h"
#include "messagehandler/message_handler.h"
#include "mock_eventbuffer.h"
#include "mock_message_handler.h"

extern int elosMessageEventPublish(elosClientConnection_t const *const conn, elosMessage_t const *const msg);
static elosEvent_t elosPublishedEvent = {0};

int elosTestElosMessageEventPublishSuccessSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);
    data->conn->isTrusted = true;
    data->conn->blacklist = (elosEventFilter_t){0};
    assert_int_equal(data->conn->blacklist.memorySize, 0);
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(data->conn->sharedData);

    data->conn->sharedData->plugin = safuAllocMem(NULL, sizeof(elosPlugin_t));
    assert_non_null(data->conn->sharedData->plugin);

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
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    free(data->conn);
    elosEventDelete(data->event);
    elosEventDeleteMembers(&elosPublishedEvent);
    free(data);
    return 0;
}

static safuResultE_t _mockElosPluginPublish(UNUSED struct elosPublisher *const publisher,
                                            UNUSED const elosEvent_t *const event) {
    elosEventDeepCopy(&elosPublishedEvent, event);
    return SAFU_RESULT_OK;
}

void elosTestElosMessageEventPublishSuccess(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    TEST("elosMessageEventPublish");
    SHOULD("%s", "publish event successfully");

    data->conn->sharedData->plugin->publish = _mockElosPluginPublish;

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_check(elosMessageHandlerSendJson, jobj, elosMessageEventPublishCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
    int result = elosMessageEventPublishCheckEvent(data->event, &elosPublishedEvent);
    assert_int_equal(result, 1);
}
