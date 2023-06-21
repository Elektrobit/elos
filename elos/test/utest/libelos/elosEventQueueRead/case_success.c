// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"

int elosTestElosEventQueueReadSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventQueueReadSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventQueueReadSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t *element;
    safuResultE_t result;

    TEST("elosEventQueueRead");
    SHOULD("%s", "successfully read from an EventQueue");

    PARAM("%s", "Read from EventQueue with Events");
    elosMockSendAndReceiveJsonMessage(test, test->responseStrNormal, SAFU_RESULT_OK);
    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->eventVector->elementCount, 2);
    element = safuVecGetLast(test->eventVector);
    assert_non_null(element);
    assert_true(element->messageCode == ELOS_MSG_CODE_WILDCARD_3XXX);

    elosEventVectorDelete(test->eventVector);

    PARAM("%s", "Read from EventQueue with no Events");
    elosMockSendAndReceiveJsonMessage(test, test->responseStrEmpty, SAFU_RESULT_OK);
    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(test->eventVector);
}
