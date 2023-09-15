// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elosSendJsonMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosSendJsonMessageErrCallSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosSendJsonMessageErrCallTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendJsonMessageErrCall(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosTestSet_t const *testSet = &test->normal;
    safuResultE_t result;

    TEST("elosSendJsonMessage");
    SHOULD("%s", "test correct behaviour of elosSendJsonMessage with sending errors");

    PARAM("%s", "json_object_to_json_string_ext fails");

    MOCK_FUNC_AFTER_CALL(json_object_to_json_string_ext, 0);
    expect_not_value(__wrap_json_object_to_json_string_ext, obj, NULL);
    expect_value(__wrap_json_object_to_json_string_ext, flags, JSON_C_TO_STRING_PLAIN);
    will_return(__wrap_json_object_to_json_string_ext, NULL);

    result = elosSendJsonMessage(&test->session, testSet->message->message, testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "elosCreateMessage fails");

    MOCK_FUNC_AFTER_CALL(elosCreateMessage, 0);
    expect_value(elosCreateMessage, id, testSet->message->message);
    expect_string(elosCreateMessage, jsonStr, testSet->jsonStr);
    expect_not_value(elosCreateMessage, message, NULL);
    will_set_parameter(elosCreateMessage, message, NULL);
    will_return(elosCreateMessage, SAFU_RESULT_FAILED);

    result = elosSendJsonMessage(&test->session, testSet->message->message, testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "elosSendMessage fails");

    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_not_value(elosSendMessage, session, NULL);
    expect_memory(elosSendMessage, message, testSet->message, testSet->messageSize);
    will_return(elosSendMessage, SAFU_RESULT_FAILED);

    result = elosSendJsonMessage(&test->session, testSet->message->message, testSet->jsonObject);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
