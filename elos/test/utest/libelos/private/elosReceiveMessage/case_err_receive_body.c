// SPDX-License-Identifier: MIT

#include "elosReceiveMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveMessageErrReceiveBodySetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosReceiveMessageErrReceiveBodyTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveMessageErrReceiveBody(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosReceiveSet_t testSet;
    safuResultE_t result;

    TEST("elosReceiveMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveMessage with receive body errors");

    PARAM("%s", "safuRecvExactly body failed");
    testSet = test->receiveJson;
    testSet.body.result = testSet.body.len / 2;
    elosMockReceiveExactlySetup(&testSet.header, &testSet.body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly body with unexpected connection close");
    testSet = test->receiveJson;
    testSet.body.result = 0;
    elosMockReceiveExactlySetup(&testSet.header, &testSet.body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly body with zero bytes");
    testSet = test->receiveJson;
    testSet.body.result = testSet.body.len / 2;
    elosMockReceiveExactlySetup(&testSet.header, &testSet.body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly body with too few bytes");
    testSet = test->receiveJson;
    testSet.body.result = testSet.body.len / 2;
    elosMockReceiveExactlySetup(&testSet.header, &testSet.body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly body with too many bytes");
    testSet = test->receiveJson;
    testSet.body.result = testSet.body.len * 2;
    elosMockReceiveExactlySetup(&testSet.header, &testSet.body);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    MOCK_FUNC_NEVER(safuRecvExactly);
}
