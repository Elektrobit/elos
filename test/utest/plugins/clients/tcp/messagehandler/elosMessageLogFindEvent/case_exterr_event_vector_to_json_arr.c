// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "mock_LogAggregator.h"
#include "mock_event_vector.h"
#include "mock_message_handler.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrEventVectorToJsonArrSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrEventVectorToJsonArrTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrEventVectorToJsonArr(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventVectorToJsonArray fails");

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorFindEvents, 0);
    expect_value(elosLogAggregatorFindEvents, logAggregator, conn->sharedData->logAggregator);
    expect_string(elosLogAggregatorFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_not_value(elosLogAggregatorFindEvents, events, NULL);
    will_return(elosLogAggregatorFindEvents, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventVectorToJsonArray, 0);
    expect_not_value(elosEventVectorToJsonArray, eventVector, NULL);
    expect_not_value(elosEventVectorToJsonArray, jEventVector, NULL);
    will_set_parameter(elosEventVectorToJsonArray, jEventVector, NULL);
    will_return(elosEventVectorToJsonArray, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_value(elosMessageHandlerSend, conn, conn);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT);
    expect_string(elosMessageHandlerSend, jsonStr, "{\"error\":\"Failed to convert event vector to json\"}");
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_OK);
}
