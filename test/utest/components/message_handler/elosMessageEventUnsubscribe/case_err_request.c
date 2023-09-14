// SPDX-License-Identifier: MIT

#include "elosMessageEventUnsubscribe_utest.h"

int elosTestElosMessageEventUnsubscribeErrRequestSetup(void **state) {
    elosMessageEventUnsubscribeUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventUnsubscribeErrRequestTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventUnsubscribeErrRequest(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *errStr;
    safuResultE_t result;

    TEST("elosMessageEventUnsubscribe");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when request parsing fails");

    MOCK_FUNC_ALWAYS(elosMessageHandlerSend);

    PARAM("%s", "json_tokener_parse fails");
    elosMessageEventUnsubscribeUtestSetupMessage(state, "#invalidJsonString#");
    errStr = _ERRSTR("Parsing the message's json string failed");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "safuJsonGetUint32 fails");
    elosMessageEventUnsubscribeUtestSetupMessage(state, "{}");
    errStr = _ERRSTR("Could not find field 'eventQueueId' in the message's json string");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Invalid eventQueueId (value 0)");
    elosMessageEventUnsubscribeUtestSetupMessage(state, "{\"eventQueueId\" : 0}");
    errStr = _ERRSTR("Invalid value for 'eventQueueId'");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Invalid eventQueueId (string instead of integer)");
    elosMessageEventUnsubscribeUtestSetupMessage(state, "{\"eventQueueId\" : \"testString\"}");
    errStr = _ERRSTR("Invalid value for 'eventQueueId'");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(elosMessageHandlerSend);
}
