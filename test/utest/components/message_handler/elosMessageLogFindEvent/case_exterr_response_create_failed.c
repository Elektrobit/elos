// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "mock_LogAggregator.h"
#include "mock_event_vector.h"
#include "mock_message_handler.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrResponseCreateFailedSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrResponseCreateFailedTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrResponseCreateFailed(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientConnection_t *connection = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "mock sending an event message successfully");

    json_object *mockJarr = __real_json_object_new_object();
    assert_non_null(mockJarr);

    MOCK_FUNC_AFTER_CALL(elosLogAggregatorFindEvents, 0);
    expect_value(elosLogAggregatorFindEvents, logAggregator, connection->sharedData->logAggregator);
    expect_string(elosLogAggregatorFindEvents, rule, ".event.name 'sshd' STRCMP");
    expect_not_value(elosLogAggregatorFindEvents, events, NULL);
    will_return(elosLogAggregatorFindEvents, SAFU_RESULT_OK);

    /* Mocking elosEventVectorToJsonArray() is the easiest way, because
    the alternative, with elosLogAggregatorFindEvents returning a eventlist,
    would need to mock elosEventVectorInitialize() instead plus the creation
    of a handwritten valid eventlist.
    */
    MOCK_FUNC_AFTER_CALL(elosEventVectorToJsonArray, 0);
    expect_not_value(elosEventVectorToJsonArray, eventVector, NULL);
    expect_not_value(elosEventVectorToJsonArray, jEventVector, NULL);
    will_set_parameter(elosEventVectorToJsonArray, jEventVector, mockJarr);
    will_return(elosEventVectorToJsonArray, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0);
    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, NULL);

    result = elosMessageLogFindEvent(connection, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
