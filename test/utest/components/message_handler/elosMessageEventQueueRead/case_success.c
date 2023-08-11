// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "elos/eventprocessor/eventprocessor.h"
#include "elosMessageEventQueueRead_utest.h"
#include "mock_message_handler.h"

int elosTestElosMessageEventQueueReadSuccessSetup(void **state) {
    char const messageString[] = "{ \"eventQueueId\" : 1 }";

    elosMessageEventQueueReadUtestSetupEventProcessor(state);
    elosMessageEventQueueReadUtestSetupMessage(state, messageString);

    return 0;
}

int elosTestElosMessageEventQueueReadSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventQueueReadSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const expectedStrData[] =
        "{\"error\":null,\"eventArray\":[{\"source\":{},\"messageCode\":2000},{\"source\":{},\"messageCode\":3000}]}";
    char const expectedStrEmpty[] = "{\"error\":null,\"eventArray\":[]}";
    safuResultE_t result;

    PARAM("%s", "elosMessageEventQueueRead with filled EventQueue");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, expectedStrData);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "elosMessageEventQueueRead with empty EventQueue");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ);
    expect_string(elosMessageHandlerSend, jsonStr, expectedStrEmpty);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventQueueRead(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
}
