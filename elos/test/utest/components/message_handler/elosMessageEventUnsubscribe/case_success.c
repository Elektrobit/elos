// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "elosMessageEventUnsubscribe_utest.h"
#include "mock_message_handler.h"

int elosTestElosMessageEventUnsubscribeSuccessSetup(void **state) {
    char const messageString[] = "{ \"eventQueueId\" : 1 }";

    elosMessageEventUnsubscribeUtestSetupEventProcessor(state);
    elosMessageEventUnsubscribeUtestSetupMessage(state, messageString);

    return 0;
}

int elosTestElosMessageEventUnsubscribeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventUnsubscribeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    const char expected[] = "{\"error\":null}";
    safuResultE_t result;

    PARAM("%s", "elosMessageEventUnsubscribe");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, expected);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
}
