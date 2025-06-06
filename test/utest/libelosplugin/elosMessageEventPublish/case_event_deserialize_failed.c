// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/mock_log.h>

#include "elos/libelosplugin/message_handler.h"
#include "elosMessageEventPublish_utest.h"
#include "mock_event.h"
#include "mock_message_handler.h"

extern int elosMessageEventPublish(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishEventEmptySetup(UNUSED void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);
    data->conn->isTrusted = true;
    data->conn->blacklist = (elosEventFilter_t){0};
    assert_int_equal(data->conn->blacklist.memorySize, 0);
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientConnectionSharedData_t));
    assert_non_null(data->conn->sharedData);

    data->response = elosMessageHandlerResponseCreate("elosEventDeserialize failed");
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

int elosTestElosMessageEventPublishEventEmptyTeardown(UNUSED void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData);
    free(data->conn);
    elosEventDelete(data->event);
    free(data);

    return 0;
}

void elosTestElosMessageEventPublishEventEmpty(UNUSED void **state) {
    elosUtestState_t *data = *state;
    safuResultE_t ret;
    const char *errstr = "elosEventDeserialize failed";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosEventDeserialize fails");

    MOCK_FUNC_ALWAYS(safuLogFunc)

    MOCK_FUNC_AFTER_CALL(elosEventDeserialize, 0);
    expect_any(elosEventDeserialize, to);
    expect_any(elosEventDeserialize, fromJsonString);
    will_return(elosEventDeserialize, SAFU_RESULT_FAILED);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, errstr);
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_check(elosMessageHandlerSendJson, jobj, elosMessageEventPublishCheckJsonObject, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageEventPublish(data->conn, data->msg);

    MOCK_FUNC_NEVER(safuLogFunc)

    assert_int_equal(ret, SAFU_RESULT_FAILED);
}
