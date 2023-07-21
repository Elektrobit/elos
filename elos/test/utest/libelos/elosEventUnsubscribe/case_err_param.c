// SPDX-License-Identifier: MIT

#include "elosEventUnsubscribe_utest.h"

#define _TEST_COMBINATIONS 3

void elosTestElosEventUnsubscribeErrParam(void **state) {
    safuResultE_t result;
    elosTestState_t *test = *state;
    elosSession_t invalidSession = {0};

    TEST("elosEventUnsubscribe (parameter error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    for (uint32_t i = 0; i < _TEST_COMBINATIONS; i++) {
        elosSession_t *session = (i & (1 << 0)) ? &test->session : NULL;
        elosEventQueueId_t eventQueueId = (i & (1 << 1)) ? _MOCK_EVENTQUEUE_ID : ELOS_ID_INVALID;

        result = elosEventUnsubscribe(session, eventQueueId);
        assert_true(result == SAFU_RESULT_FAILED);

        // First call of unsubscribe will reset session.
        test->session.connected = true;
    }

    PARAM("%s", "test correct behaviour with invalid session");
    result = elosEventUnsubscribe(&invalidSession, _MOCK_EVENTQUEUE_ID);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
