// SPDX-License-Identifier: MIT
#include "elosSendMessage_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosSendMessageErrParam),
        TEST_CASE(elosTestElosSendMessageErrSend),
        TEST_CASE(elosTestElosSendMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    test->session.connected = true;

    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, "{}", &test->message);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosCreateMessage(ELOS_MESSAGE_GET_VERSION, NULL, &test->messageNoJson);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    free(test->messageNoJson);
    free(*state);

    return 0;
}
