// SPDX-License-Identifier: MIT
#include "elosSendJsonMessage_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosSendJsonMessageErrCall),
        TEST_CASE(elosTestElosSendJsonMessageErrParam),
        TEST_CASE(elosTestElosSendJsonMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    test->normal.jsonStr = "{\"payload\":\"test123\"}";
    test->normal.jsonObject = json_tokener_parse(test->normal.jsonStr);
    assert_non_null(test->normal.jsonObject);
    test->normal.messageSize = sizeof(elosMessage_t) + strlen(test->normal.jsonStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, test->normal.jsonStr, &test->normal.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->empty.jsonStr = "{}";
    test->empty.jsonObject = json_tokener_parse(test->empty.jsonStr);
    assert_non_null(test->empty.jsonObject);
    test->empty.messageSize = sizeof(elosMessage_t) + strlen(test->empty.jsonStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, test->empty.jsonStr, &test->empty.message);
    assert_int_equal(result, SAFU_RESULT_OK);

    test->session.connected = true;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    json_object_put(test->normal.jsonObject);
    json_object_put(test->empty.jsonObject);
    free(test->normal.message);
    free(test->empty.message);
    free(*state);

    return 0;
}
