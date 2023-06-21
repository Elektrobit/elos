// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elosReceiveJsonMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveJsonMessageErrJsonSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    test->normal.jsonObject = NULL;
    test->error.jsonObject = NULL;

    return 0;
}

int elosTestElosReceiveJsonMessageErrJsonTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveJsonMessageErrJson(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t *testSet;
    safuResultE_t result;

    TEST("elosReceiveJsonMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveJsonMessage with json errors");

    PARAM("%s", "json_tokener_parse fails");

    MOCK_FUNC_AFTER_CALL(json_tokener_parse, 0);
    expect_not_value(__wrap_json_tokener_parse, str, NULL);
    will_return(__wrap_json_tokener_parse, NULL);

    testSet = &test->normal;
    elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(testSet->jsonObject);

    // FIXME: Taken out due to inconsistent response generation, see #15517 for more details
    PARAM("%s", "SKIPPED: json_object_object_get_ex fails (#15517)");
    // PARAM("%s", "json_object_object_get_ex fails");

    // MOCK_FUNC_AFTER_CALL(json_object_object_get_ex, 0);
    // expect_not_value(__wrap_json_object_object_get_ex, obj, NULL);
    // expect_string(__wrap_json_object_object_get_ex, key, "error");
    // expect_not_value(__wrap_json_object_object_get_ex, value, NULL);
    // will_return(__wrap_json_object_object_get_ex, false);

    // testSet = &test->normal;
    // elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    // result = elosReceiveJsonMessage(&test->session, testSet->message->message, &testSet->jsonObject);
    // assert_int_equal(result, SAFU_RESULT_FAILED);
    // assert_null(testSet->jsonObject);

    PARAM("%s", "Received message has error set");

    testSet = &test->error;
    elosMockReceive(test, testSet->message, SAFU_RESULT_OK);
    result = elosReceiveJsonMessage(&test->session, testSet->message->message, &testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(testSet->jsonObject);
}
