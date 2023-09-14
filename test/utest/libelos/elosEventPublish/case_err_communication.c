// SPDX-License-Identifier: MIT

#include "elosEventPublish_utest.h"

int elosTestElosEventPublishErrCommunicationSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventPublishErrCommunicationTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventPublishErrCommunication(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosUnitTestSet_t *tSet;
    safuResultE_t result;

    TEST("elosEventPublish (communication error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failing communcation with elosd");

    tSet = &test->normal;
    elosMockSendAndReceiveJsonMessage(test, tSet, SAFU_RESULT_FAILED);
    result = elosEventPublish(&test->session, &tSet->event);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    MOCK_FUNC_NEVER(safuRecvExactly);
}
