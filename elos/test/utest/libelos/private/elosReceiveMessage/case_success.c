// SPDX-License-Identifier: MIT

#include "elosReceiveMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveMessageSuccessSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosReceiveMessageSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    test->message = NULL;

    return 0;
}

void elosTestElosReceiveMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosReceiveSet_t const *testSet;
    safuResultE_t result;

    TEST("elosReceiveMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveMessage");

    PARAM("%s", "receive message without jsonStr");
    testSet = &test->receiveNoJson;
    elosMockReceiveExactlySetup(&testSet->header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_memory_equal(test->message, testSet->message, testSet->messageSize);
    free(test->message);
    test->message = NULL;

    PARAM("%s", "receive message with jsonStr");
    testSet = &test->receiveJson;
    elosMockReceiveExactlySetup(&testSet->header, &testSet->body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_memory_equal(test->message, testSet->message, testSet->messageSize);
    free(test->message);
    test->message = NULL;
}
