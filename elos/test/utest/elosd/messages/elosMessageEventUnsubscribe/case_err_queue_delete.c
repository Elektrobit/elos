// SPDX-License-Identifier: MIT

#include "elosMessageEventUnsubscribe_utest.h"

int elosTestElosMessageEventUnsubscribeErrQueueDeleteSetup(void **state) {
    elosMessageEventUnsubscribeUtestSetupEventProcessor(state);

    return 0;
}

int elosTestElosMessageEventUnsubscribeErrQueueDeleteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventProcessorDeleteMembers(&test->eventProcessor);

    return 0;
}

void elosTestElosMessageEventUnsubscribeErrQueueDelete(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *errStr;
    safuResultE_t result;

    TEST("elosMessageEventUnsubscribe");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when deleting the event queue failed");

    MOCK_FUNC_AFTER_CALL(elosEventFilterManagerNodeRemoveByEventQueueId, 0);
    expect_not_value(elosEventFilterManagerNodeRemoveByEventQueueId, efm, NULL);
    expect_value(elosEventFilterManagerNodeRemoveByEventQueueId, eqId, 2);
    will_return(elosEventFilterManagerNodeRemoveByEventQueueId, SAFU_RESULT_OK);

    elosMessageEventUnsubscribeUtestSetupMessage(state, "{\"eventQueueId\" : 2}");
    errStr = _ERRSTR("Removing the EventQueue failed");
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);
    expect_not_value(elosMessageHandlerSend, conn, NULL);
    expect_value(elosMessageHandlerSend, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
    expect_string(elosMessageHandlerSend, jsonStr, errStr);
    will_return(elosMessageHandlerSend, SAFU_RESULT_OK);

    result = elosMessageEventUnsubscribe(&test->connection, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
