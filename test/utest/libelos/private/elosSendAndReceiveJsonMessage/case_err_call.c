// SPDX-License-Identifier: MIT

#include "elosSendAndReceiveJsonMessage_utest.h"

int elosTestElosSendAndReceiveJsonMessageErrCallSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendAndReceiveJsonMessageErrCallTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendAndReceiveJsonMessageErrCall(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t *tSet = &test->normal;
    safuResultE_t result;

    TEST("elosSendAndReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour of elosSendAndReceiveJsonMessage during errors");

    PARAM("%s", "elosSendJsonMessage fails");
    elosMockSendJsonMessage(test, tSet->requestId, SAFU_RESULT_FAILED);
    result = elosSendAndReceiveJsonMessage(&test->session, tSet->requestId, tSet->requestObj, &test->bufferObj);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosReceiveJsonMessage fails");
    elosMockSendJsonMessage(test, tSet->requestId, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, tSet->responseId, NULL, SAFU_RESULT_FAILED);
    result = elosSendAndReceiveJsonMessage(&test->session, tSet->requestId, tSet->requestObj, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
