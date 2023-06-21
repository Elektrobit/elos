// SPDX-License-Identifier: MIT

#include "elosMessageEventQueueRead_utest.h"

int elosTestElosMessageEventQueueReadErrRequestSetup(void **state) {
    elosMessageEventQueueReadUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventQueueReadErrRequestTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventQueueReadErrRequest(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *errStr;
    safuResultE_t result;

    TEST("elosMessageEventQueueRead");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when request parsing fails");

    MOCK_FUNC_ALWAYS(elosMessageHandlerSend);

    PARAM("%s", "json_tokener_parse fails");
    elosMessageEventQueueReadUtestSetupMessage(state, "#invalidJsonString#");
    errStr = _ERRSTR("Parsing the message's json string failed");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "safuJsonGetUint32 fails");
    elosMessageEventQueueReadUtestSetupMessage(state, "{}");
    errStr = _ERRSTR("Could not find field 'eventQueueId' in the message's json string");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Invalid eventQueueId (value 0)");
    elosMessageEventQueueReadUtestSetupMessage(state, "{\"eventQueueId\" : 0}");
    errStr = _ERRSTR("Invalid value for 'eventQueueId'");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Invalid eventQueueId (string instead of integer)");
    elosMessageEventQueueReadUtestSetupMessage(state, "{\"eventQueueId\" : \"testString\"}");
    errStr = _ERRSTR("Invalid value for 'eventQueueId'");
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);
    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(elosMessageHandlerSend);
}
