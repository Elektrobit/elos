// SPDX-License-Identifier: MIT

#include <errno.h>

#include "elosReceiveMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveMessageErrReceiveHeaderSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosReceiveMessageErrReceiveHeaderTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveMessageErrReceiveHeader(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosReceiveSet_t testSet;
    safuResultE_t result;

    TEST("elosReceiveMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveMessage with receive errors");

    PARAM("%s", "safuRecvExactly header failed");
    testSet = test->receiveNoJson;
    testSet.header.result = -1;
    errno = ENETDOWN;
    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly header with unexpected connection close");
    testSet = test->receiveNoJson;
    testSet.header.result = 0;
    errno = 0;
    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly header with zero bytes and errno set");
    testSet = test->receiveNoJson;
    testSet.header.result = 0;
    errno = ENETDOWN;
    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly header with too few bytes");
    testSet = test->receiveNoJson;
    testSet.header.result -= 1;
    errno = ENETDOWN;
    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuRecvExactly header with too many bytes");
    testSet = test->receiveNoJson;
    testSet.header.result += 1;
    errno = 0;
    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);
}
