// SPDX-License-Identifier: MIT

#include "elosEventUnsubscribe_utest.h"

void elosTestElosEventUnsubscribeErrCommunication(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventUnsubscribe (communication error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failing communcation with elosd");

    elosMockSendAndReceiveJsonMessage(test, "{\"error\":\"Communication with elosd failed.\"}", SAFU_RESULT_FAILED);
    result = elosEventUnsubscribe(&test->session, _MOCK_EVENTQUEUE_ID);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
