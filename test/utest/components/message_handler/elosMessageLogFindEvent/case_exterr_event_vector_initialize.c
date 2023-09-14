// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "mock_event_vector.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrEventVectorInitializeSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrEventVectorInitializeTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrEventVectorInitialize(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientManagerConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventVectorInitialize fails");

    MOCK_FUNC_AFTER_CALL(elosEventVectorInitialize, 0);
    expect_not_value(elosEventVectorInitialize, eventVector, NULL);
    expect_any(elosEventVectorInitialize, elements);
    will_return(elosEventVectorInitialize, SAFU_RESULT_FAILED);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}