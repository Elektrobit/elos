// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elosReceiveJsonMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveJsonMessageErrReceiveSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosReceiveJsonMessageErrReceiveTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveJsonMessageErrReceive(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t *testSet = &test->normal;
    safuResultE_t result;

    TEST("elosReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveJsonMessage with receive errors");

    PARAM("%s", "elosReceiveMessage fails");

    elosMockReceive(test, testSet->message, SAFU_RESULT_FAILED);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, &testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Message has wrong id");

    result = elosMessageClone(&test->clonedMessage, testSet->message);
    assert_int_equal(result, SAFU_RESULT_OK);
    test->clonedMessage->message += 1;

    MOCK_FUNC_AFTER_CALL(elosReceiveMessage, 0);
    expect_value(elosReceiveMessage, session, &test->session);
    expect_not_value(elosReceiveMessage, message, NULL);
    will_set_parameter(elosReceiveMessage, message, test->clonedMessage);
    will_return(elosReceiveMessage, SAFU_RESULT_OK);

    result = elosReceiveJsonMessage(&test->session, testSet->message->message, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "Message has no payload");

    testSet = &test->nojson;
    elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
