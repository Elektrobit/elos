// SPDX-License-Identifier: MIT

#include <bits/time.h>
#include <cmocka_extensions/mock_extensions.h>
#include <time.h>

#include "cmocka_mocks/mock_libc.h"
#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter.h"
#include "elosMessageEventPublish_utest.h"
#include "json-c/json.h"
#include "mock_LogAggregator.h"
#include "mock_event.h"
#include "mock_eventfilter.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/mock_safu.h"

extern int elosMessageEventPublish(elosClientManagerConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestelosMessageEventPublishBlacklistFilterSuccessSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    assert_non_null(data->conn);

    data->conn->isTrusted = false;
    data->conn->blacklist = (elosEventFilter_t){0};
    const elosEventFilterParam_t param = {.filterString = ".event.messageCode 200 EQ"};
    result = elosEventFilterCreate(&data->conn->blacklist, &param);
    assert_int_equal(result, RPNFILTER_RESULT_OK);

    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    assert_non_null(data->conn->sharedData);
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    assert_non_null(data->conn->sharedData->eventProcessor);

    const char mockErrString[] = "unauthorized publishing attempt";
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

int elosTestelosMessageEventPublishBlacklistFilterSuccessTeardown(void **state) {
    elosUtestState_t *data = *state;
    json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    elosEventFilterDeleteMembers(&data->conn->blacklist);
    free(data->conn);
    elosEventDelete(data->event);
    free(data);
    return 0;
}

static int _check_json_object(const long unsigned int parameter, const long unsigned int compare) {
    json_object *jobjParam = (json_object *)parameter;
    json_object *jobjCmp = (json_object *)compare;
    if (json_object_equal(jobjParam, jobjCmp)) {
        return 1;
    }
    print_error("%s != %s\n", json_object_to_json_string(jobjParam), json_object_to_json_string(jobjCmp));
    return 0;
}

static int _check_event(const long unsigned int parameter, const long unsigned int compare) {
    int result = 1;
    json_object *eventParam = json_object_new_object();
    json_object *eventCmp = json_object_new_object();
    safuResultE_t res = elosEventToJsonObject(eventParam, (elosEvent_t *)parameter);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    res = elosEventToJsonObject(eventCmp, (elosEvent_t *)compare);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    if (result == 1) {
        result = _check_json_object((const long unsigned int)eventParam, (const long unsigned int)eventCmp);
    }

    json_object_put(eventParam);
    json_object_put(eventCmp);

    return result;
}

void elosTestelosMessageEventPublishBlacklistFilterSuccess(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    const elosEvent_t securityEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {.appName = "elosd", .fileName = "/usr/bin/elosd", .pid = 100},
        .severity = ELOS_SEVERITY_WARN,
        .hardwareid = "localhost",
        .classification = ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC,
        .messageCode = 8007,
        .payload = data->msg->json,
    };

    TEST("elosmessageEventPublish");
    SHOULD("%s", "publish a security event successfully");

    MOCK_FUNC_ENABLE(clock_gettime);
    expect_any(__wrap_clock_gettime, clock_id);
    expect_any(__wrap_clock_gettime, tp);
    will_set_parameter(__wrap_clock_gettime, tp, &securityEvent.date);
    will_return(__wrap_clock_gettime, 0);

    MOCK_FUNC_ENABLE(getpid);
    will_return(__wrap_getpid, 100);

    MOCK_FUNC_ENABLE(safuGetHardwareId);
    will_return(__wrap_safuGetHardwareId, "localhost");

    MOCK_FUNC_ENABLE(elosEventProcessorPublish);
    expect_value(elosEventProcessorPublish, eventProcessor, data->conn->sharedData->eventProcessor);
    expect_check(elosEventProcessorPublish, event, _check_event, &securityEvent);
    will_return(elosEventProcessorPublish, SAFU_RESULT_OK);

    MOCK_FUNC_ENABLE(elosLogAggregatorAdd);
    expect_value(elosLogAggregatorAdd, logAggregator, data->conn->sharedData->logAggregator);
    expect_check(elosLogAggregatorAdd, event, _check_event, &securityEvent);
    will_return(elosLogAggregatorAdd, SAFU_RESULT_OK);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson)
    expect_any(elosMessageHandlerSendJson, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH);
    expect_check(elosMessageHandlerSendJson, jobj, _check_json_object, data->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_OK);
}
