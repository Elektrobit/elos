// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"

int elosTestElosEventQueueReadErrExchangeSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventQueueReadErrExchangeTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventQueueReadErrExchange(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventQueueRead (communication error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failing communcation with elosd");

    elosMockSendAndReceiveJsonMessage(test, test->responseStrNormal, SAFU_RESULT_FAILED);
    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
