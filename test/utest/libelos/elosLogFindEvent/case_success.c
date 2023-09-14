// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"
#include "safu/mock_safu.h"

int elosTestElosLogFindEventSuccessSetup(void **state) {
    static elosUteststateT_t testState = {0};

    const char requestPayload[] = "{\"filter\":\".event.name 'sshd' STRCMP\"}";
    elosLogFindEventUtestCreateMessage(&testState.request, &testState.requestLength, ELOS_MESSAGE_LOG_FIND_EVENT,
                                       requestPayload, strlen(requestPayload));

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    elosLogFindEventUtestCreateResponse(state);
    return 0;
}

int elosTestElosLogFindEventSuccessTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosLogFindEventUtestFreeSession(state);
    elosLogFindEventUtestDeleteMessage(testState->request);
    elosLogFindEventUtestDeleteResponse(state);
    return 0;
}

void elosTestElosLogFindEventSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *eventVector = NULL;
    elosEvent_t *event;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_OK if sending and receiving was successful");

    mockSession->fd = VALID_SESSION_FD;
    mockSession->connected = true;

    elosMockSendAndReceiveJsonMessage(testState, MOCK_RESPONSE_JOBJ, SAFU_RESULT_OK);

    result = elosLogFindEvent(mockSession, filterRule, &eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(eventVector);
    assert_int_equal(eventVector->elementCount, 2);
    event = safuVecGet(eventVector, 0);
    assert_int_equal(event->messageCode, MOCK_MESSAGECODE);
    event = safuVecGetLast(eventVector);
    assert_string_equal(event->payload, MOCK_PAYLOAD);

    elosEventVectorDelete(eventVector);
}
