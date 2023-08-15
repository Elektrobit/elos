// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "mock_LogAggregator.h"
#include "mock_event_vector.h"
#include "mock_message_handler.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventSuccessSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);

    char filterRule[] = "{\"filter\": \".event.payload 'sshd' STRCMP\"}";
    size_t ruleLen = strlen(filterRule) + 1;

    elosMessage_t *msg = test_malloc(sizeof(elosMessage_t) + ruleLen);
    assert_non_null(msg);

    msg->version = 0;
    msg->message = ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT;
    msg->length = ruleLen;
    strcpy(msg->json, filterRule);

    testState.msg = msg;

    return 0;
}

int elosTestElosMessageLogFindEventSuccessTeardown(void **state) {
    elosUteststateT_t *data = (elosUteststateT_t *)*state;
    json_object_put(data->response);
    elosMessageLogFindEventUtestFreeConnection(state);
    test_free(data->msg);

    return 0;
}

static int _checkJsonObject(const long unsigned int jsonParam, const long unsigned int jsonExpected) {
    json_object *param = (json_object *)jsonParam;
    json_object *expected = (json_object *)jsonExpected;
    if (NULL == param) {
        print_error("Parameter is NULL\n");
        return 0;
    }
    if (NULL == expected) {
        print_error("Expected NULL but found %s\n", json_object_to_json_string(param));
        return 0;
    }
    if (!json_object_equal(param, expected)) {
        print_message("%s != %s\n", json_object_to_json_string(param), json_object_to_json_string(expected));
        return 0;
    }
    return 1;
}

void elosTestElosMessageLogFindEventSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientConnection_t *connection = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "mock sending an event message successfully");

    testState->response = json_tokener_parse("{ \"error\": null, \"eventArray\": [ ] }");

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorFindEvents, 0);
    expect_value(elosLogAggregatorFindEvents, logAggregator, connection->sharedData->logAggregator);
    expect_string(elosLogAggregatorFindEvents, rule, ".event.payload 'sshd' STRCMP");
    expect_not_value(elosLogAggregatorFindEvents, events, NULL);
    will_return(elosLogAggregatorFindEvents, SAFU_RESULT_OK);

    MOCK_FUNC_ENABLE(elosMessageHandlerSendJson);
    expect_value(elosMessageHandlerSendJson, conn, connection);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT);
    expect_check(elosMessageHandlerSendJson, jobj, _checkJsonObject, testState->response);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);

    result = elosMessageLogFindEvent(connection, msg);

    assert_int_equal(result, SAFU_RESULT_OK);
}
