// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventExtErrCommunicationSetup(void **state) {
    static elosUteststateT_t testState = {0};

    const char payload[] = "{\"filter\":\".event.name 'sshd' STRCMP\"}";

    elosLogFindEventUtestCreateMessage(&testState.request, &testState.requestLength, ELOS_MESSAGE_LOG_FIND_EVENT,
                                       payload, strlen(payload));

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    return 0;
}

int elosTestElosLogFindEventExtErrCommunicationTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosLogFindEventUtestFreeSession(state);
    elosLogFindEventUtestDeleteMessage(testState->request);
    return 0;
}

void elosTestElosLogFindEventExtErrCommunication(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    mockSession->connected = true;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosSendAndReceiveJsonMessage failed");

    mockSession->fd = VALID_SESSION_FD;

    elosMockSendAndReceiveJsonMessage(testState, NULL, SAFU_RESULT_FAILED);

    result = elosLogFindEvent(mockSession, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
