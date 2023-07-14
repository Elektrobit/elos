// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "mock_LogAggregator.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrLogAggregatorFindEventsSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrLogAggregatorFindEventsTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrLogAggregatorFindEvents(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientManagerConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosLogAggregatorFindEvents fails");

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorFindEvents, 0);
    expect_value(elosLogAggregatorFindEvents, logAggregator, conn->sharedData->logAggregator);
    expect_string(elosLogAggregatorFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_not_value(elosLogAggregatorFindEvents, events, NULL);
    will_return(elosLogAggregatorFindEvents, SAFU_RESULT_FAILED);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
