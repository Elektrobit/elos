// SPDX-License-Identifier: MIT

#include "elosReceiveJsonMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveJsonMessageSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosReceiveJsonMessageSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    json_object_put(test->normal.jsonObject);
    test->normal.jsonObject = NULL;
    test->clonedMessage = NULL;

    return 0;
}

void elosTestElosReceiveJsonMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t *testSet = &test->normal;
    safuResultE_t result;
    char const *jsonStr;

    TEST("elosReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveJsonMessage");

    PARAM("%s", "receive, check and return message");
    elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, &testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_OK);
    jsonStr = json_object_to_json_string_ext(testSet->jsonObject, JSON_C_TO_STRING_PLAIN);
    assert_non_null(jsonStr);
    assert_string_equal(jsonStr, testSet->jsonStr);

    PARAM("%s", "receive, check and discard message");
    elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, NULL);
    assert_int_equal(result, SAFU_RESULT_OK);
}
