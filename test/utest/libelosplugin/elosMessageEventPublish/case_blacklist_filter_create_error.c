// SPDX-License-Identifier: MIT

#include <bits/time.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <json-c/json.h>
#include <safu/common.h>
#include <safu/mock_log.h>
#include <safu/mock_safu.h>
#include <time.h>

#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/libelosplugin/message_handler.h"
#include "elosMessageEventPublish_utest.h"
#include "mock_eventbuffer.h"
#include "mock_logger.h"
#include "mock_message_handler.h"

extern int elosMessageEventPublish(elosClientConnection_t const *const conn, elosMessage_t const *const msg);
static elosEvent_t elosPublishedEvent = {0};
static elosEvent_t elosStoredEvent = {0};

int elosTestelosMessageEventPublishBlacklistFilterCreateErrorSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);

    data->conn->isTrusted = false;
    data->conn->blacklist = (elosEventFilter_t){0};

    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(data->conn->sharedData);

    data->conn->sharedData->plugin = safuAllocMem(NULL, sizeof(elosPlugin_t));
    assert_non_null(data->conn->sharedData->plugin);
    memset(data->conn->sharedData->plugin, 0, sizeof(elosPlugin_t));

    const char mockErrString[] = "event authorization failed";
    data->response = elosMessageHandlerResponseCreate(mockErrString);
    assert_non_null(data->response);

    const char *msg =
        "{\"topicID\": 0, \"topic\": \"test\", \"date\": [2000, 100], \"source\": \"Test\", \"severity\": 0, "
        "\"hardwareid\":\"0000\", \"classification\": 0, \"messageCode\": 200, \"payload\": \"testevent\"}";
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

int elosTestelosMessageEventPublishBlacklistFilterCreateErrorTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->plugin);
    free(data->conn->sharedData);
    elosEventFilterDeleteMembers(&data->conn->blacklist);
    free(data->conn);
    elosEventDelete(data->event);
    free(data);
    return 0;
}

static safuResultE_t _mockElosPluginPublish(UNUSED struct elosPublisher *const publisher,
                                            const elosEvent_t *const event) {
    elosEventDeepCopy(&elosPublishedEvent, event);
    return SAFU_RESULT_OK;
}

static safuResultE_t _mockElosPluginStore(UNUSED struct elosPluginControl *plugin, const elosEvent_t *const event) {
    elosEventDeepCopy(&elosStoredEvent, event);
    return SAFU_RESULT_OK;
}

void elosTestelosMessageEventPublishBlacklistFilterCreateError(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    const elosEvent_t errorEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {.appName = "elosd", .fileName = "/usr/bin/elosd", .pid = 100},
        .severity = ELOS_SEVERITY_ERROR,
        .hardwareid = "localhost",
        .classification = ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS,
        .messageCode = 501,
        .payload = data->msg->json,
    };

    TEST("elosMessageEventPublish");
    SHOULD("%s", "publish a security event successfully");

    MOCK_FUNC_ENABLE(clock_gettime);
    expect_any(__wrap_clock_gettime, clock_id);
    expect_any(__wrap_clock_gettime, tp);
    will_set_parameter(__wrap_clock_gettime, tp, &errorEvent.date);
    will_return(__wrap_clock_gettime, 0);

    MOCK_FUNC_ENABLE(getpid);
    will_return(__wrap_getpid, 100);

    MOCK_FUNC_ENABLE(safuGetHardwareId);
    will_return(__wrap_safuGetHardwareId, "localhost");

    data->conn->sharedData->plugin->publish = _mockElosPluginPublish;
    data->conn->sharedData->plugin->store = _mockElosPluginStore;

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_check(elosMessageHandlerSendJson, jobj, elosMessageEventPublishCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
    int result = elosMessageEventPublishCheckEvent(&errorEvent, &elosPublishedEvent);
    assert_int_equal(result, 1);
    result = elosMessageEventPublishCheckEvent(&errorEvent, &elosStoredEvent);
    assert_int_equal(result, 1);
}
