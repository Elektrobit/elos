// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/event/event_types.h>
#include <json-c/json.h>
#include <safu/common.h>
#include <safu/mock_log.h>
#include <safu/result.h>

#include "elos/libelosplugin/types.h"
#include "elosMessageEventPublish_utest.h"
#include "mock_event.h"
#include "mock_eventbuffer.h"
#include "mock_message_handler.h"

extern int elosMessageEventPublish(elosClientConnection_t const *const conn, elosMessage_t const *const msg);
static elosEvent_t elosPublishedEvent = {0};
static elosEvent_t elosStoredEvent = {0};

int elosTestElosMessageEventPublishPublishFailedSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);
    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);
    data->conn->isTrusted = true;
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(data->conn->sharedData);

    data->conn->sharedData->plugin = safuAllocMem(NULL, sizeof(elosPlugin_t));
    assert_non_null(data->conn->sharedData->plugin);
    memset(data->conn->sharedData->plugin, 0, sizeof(elosPlugin_t));

    data->response = json_object_new_object();
    assert_non_null(data->response);
    *state = data;
    return 0;
}

int elosTestElosMessageEventPublishPublishFailedTeardown(void **state) {
    elosUtestState_t *data = *state;
    free(data->msg);
    memset(data->conn->sharedData->plugin, 0, sizeof(elosPlugin_t));
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    free(data->conn);
    elosEventDeleteMembers(&elosPublishedEvent);
    free(data);
    return 0;
}

static safuResultE_t _mockElosPluginPublish(UNUSED struct elosPublisher *const publisher,
                                            const elosEvent_t *const event) {
    elosEventDeepCopy(&elosPublishedEvent, event);
    return SAFU_RESULT_FAILED;
}

static safuResultE_t _mockElosPluginStore(UNUSED struct elosPluginControl *plugin, const elosEvent_t *const event) {
    elosEventDeepCopy(&elosStoredEvent, event);
    return SAFU_RESULT_OK;
}

void elosTestElosMessageEventPublishPublishFailed(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    elosEvent_t expectedEvent = {0};
    const char *msg =
        "{\"date\": [2000, 100], \"source\": \"Test\", \"severity\": 0, \"hardwareid\":\"0000\","
        "\"classification\": 0, \"messageCode\": 0, \"payload\": \"testevent\"}";
    const char *errstr = "Failed to publish event";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosEventProcessorPublish fails");

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    assert_non_null(data->msg);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    data->conn->sharedData->plugin->publish = _mockElosPluginPublish;
    data->conn->sharedData->plugin->store = _mockElosPluginStore;

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

    ret = elosEventDeserialize(&expectedEvent, msg);
    int result = elosMessageEventPublishCheckEvent(&expectedEvent, &elosPublishedEvent);
    assert_int_equal(result, 1);
    result = elosMessageEventPublishCheckEvent(&expectedEvent, &elosStoredEvent);
    assert_int_equal(result, 1);
    elosEventDeleteMembers(&expectedEvent);
}
