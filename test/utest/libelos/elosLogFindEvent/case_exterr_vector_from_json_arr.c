// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"
#include "mock_event_vector.h"
#include "safu/mock_safu.h"

int elosTestElosLogFindEventExtErrVectorFromJsonArrSetup(void **state) {
    static elosUteststateT_t testState = {0};

    const char requestPayload[] = "[{\"filter\":\".event.name 'sshd' STRCMP\"}]";
    elosLogFindEventUtestCreateMessage(&testState.request, &testState.requestLength, ELOS_MESSAGE_LOG_FIND_EVENT,
                                       requestPayload, strlen(requestPayload));

    *state = &testState;
    elosLogFindEventUtestCreateSession(state);
    elosLogFindEventUtestCreateResponse(state);
    return 0;
}

int elosTestElosLogFindEventExtErrVectorFromJsonArrTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosLogFindEventUtestFreeSession(state);
    elosLogFindEventUtestDeleteMessage(testState->request);
    elosLogFindEventUtestDeleteResponse(state);
    return 0;
}

void elosTestElosLogFindEventExtErrVectorFromJsonArr(void **state) {
    elosUteststateT_t *testState = *state;
    elosSession_t *mockSession = testState->session;
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventVectorFromJsonArray returns an empty event vector");

    mockSession->fd = VALID_SESSION_FD;
    mockSession->connected = true;

    elosMockSendAndReceiveJsonMessage(testState, MOCK_RESPONSE_JOBJ, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventVectorFromJsonArray, 0);
    expect_not_value(elosEventVectorFromJsonArray, jEventArray, NULL);
    expect_not_value(elosEventVectorFromJsonArray, eventVector, NULL);
    will_set_parameter(elosEventVectorFromJsonArray, eventVector, NULL);
    will_return(elosEventVectorFromJsonArray, SAFU_RESULT_FAILED);

    result = elosLogFindEvent(mockSession, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
