// SPDX-License-Identifier: MIT

#include "elosMessageEventUnsubscribe_utest.h"
#include "mock_event_vector.h"
#include "mock_eventprocessor.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventUnsubscribeErrResponseSendSetup(void **state) {
    elosMessageEventUnsubscribeUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventUnsubscribeErrResponseSendTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventUnsubscribeErrResponseSend(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosMessageEventUnsubscribe");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when sending the response fails");

    PARAM("%s", "elosMessageHandlerSendJson fails");

    elosMessageEventUnsubscribeUtestSetupMessage(state, "{\"eventQueueId\" : 1}");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);
    expect_not_value(elosMessageHandlerSendJson, conn, NULL);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_not_value(elosMessageHandlerSendJson, jobj, NULL);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_FAILED);

    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosMessageHandlerSendJson fails (with previous error)");

    elosMessageEventUnsubscribeUtestSetupMessage(state, "#invalidJsonString#");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);
    expect_not_value(elosMessageHandlerSendJson, conn, NULL);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_not_value(elosMessageHandlerSendJson, jobj, NULL);
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_FAILED);

    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
