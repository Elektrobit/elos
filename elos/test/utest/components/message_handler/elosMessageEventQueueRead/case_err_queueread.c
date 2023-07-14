// SPDX-License-Identifier: MIT

#include "elosMessageEventQueueRead_utest.h"
#include "mock_event_vector.h"
#include "mock_eventprocessor.h"

int elosTestElosMessageEventQueueReadErrQueueReadSetup(void **state) {
    elosMessageEventQueueReadUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventQueueReadErrQueueReadTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventQueueReadErrQueueRead(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *errStr;
    safuResultE_t result;

    TEST("elosMessageEventQueueRead");
    SHOULD("%s", "Properly handle errors during exection");

    elosMessageEventQueueReadUtestSetupMessage(state, "{\"eventQueueId\" : 1}");

    PARAM("%s", "elosEventProcessorQueueRead fails");

    MOCK_FUNC_AFTER_CALL(elosEventProcessorQueueRead, 0);
    expect_not_value(elosEventProcessorQueueRead, eventProcessor, NULL);
    expect_value(elosEventProcessorQueueRead, eventQueueId, 1);
    expect_not_value(elosEventProcessorQueueRead, eventVector, NULL);
    will_set_parameter(elosEventProcessorQueueRead, eventVector, NULL);
    will_return(elosEventProcessorQueueRead, SAFU_RESULT_FAILED);

    errStr = _ERRSTR("Reading from the EventQueue failed");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosEventVectorToJsonArray fails");

    MOCK_FUNC_AFTER_CALL(elosEventVectorToJsonArray, 0);
    expect_not_value(elosEventVectorToJsonArray, eventVector, NULL);
    expect_not_value(elosEventVectorToJsonArray, jEventVector, NULL);
    will_set_parameter(elosEventVectorToJsonArray, jEventVector, NULL);
    will_return(elosEventVectorToJsonArray, SAFU_RESULT_FAILED);

    errStr = _ERRSTR("Converting the eventVector into a json array failed (events were lost)");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
