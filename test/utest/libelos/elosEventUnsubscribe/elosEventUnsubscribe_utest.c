// SPDX-License-Identifier: MIT
#include "elosEventUnsubscribe_utest.h"

#include <stdlib.h>

TEST_SUITE_FUNC_PROTOTYPES(elosEventUnsubscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(elosTestElosEventUnsubscribeErrCommunication),
        cmocka_unit_test(elosTestElosEventUnsubscribeErrParam),
        cmocka_unit_test(elosTestElosEventUnsubscribeErrRequest),
        cmocka_unit_test(elosTestElosEventUnsubscribeSuccess),
    };

    return RUN_TEST_SUITE(tests, elosEventUnsubscribeUtest);
}

static int elosEventUnsubscribeUtestSetup(UNUSED void **state) {
    elosTestState_t *test;
    safuResultE_t result;

    *state = calloc(1, sizeof(*test));
    assert_non_null(*state);

    test = *state;

    test->session.fd = _MOCK_FD;
    test->session.connected = true;

    test->requestStr = "{\"eventQueueId\":42}";
    test->requestSize = sizeof(*test->request) + strlen(test->requestStr) + 1;
    result = elosCreateMessage(ELOS_MESSAGE_EVENT_UNSUBSCRIBE, test->requestStr, &test->request);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

static int elosEventUnsubscribeUtestTeardown(UNUSED void **state) {
    elosTestState_t *test = *state;

    free(test->request);
    free(test);

    return 0;
}

void elosMockSendAndReceiveJsonMessage(elosTestState_t *test, char const *responseStr, safuResultE_t result) {
    // We do not mock elosSendAndReceiveJsonMessage directly here, as we want to check the message that is sent
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_value(elosSendMessage, session, &test->session);
    expect_memory(elosSendMessage, message, test->request, test->requestSize);
    will_return(elosSendMessage, result);

    if (result == SAFU_RESULT_OK) {
        json_object *responseObj = NULL;

        // elosLogFindEvent frees the jsonObject it created, so we have to give it one to free
        responseObj = json_tokener_parse(responseStr);
        assert_non_null(responseObj);

        MOCK_FUNC_AFTER_CALL(elosReceiveJsonMessage, 0);
        expect_value(elosReceiveJsonMessage, session, &test->session);
        expect_value(elosReceiveJsonMessage, messageId, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE);
        expect_not_value(elosReceiveJsonMessage, jsonObject, NULL);
        will_set_parameter(elosReceiveJsonMessage, jsonObject, responseObj);
        will_return(elosReceiveJsonMessage, result);

        // MOCK_FUNC_AFTER_CALL(elosReceiveMessage, 0);
        // expect_value(elosReceiveMessage, session, &test->session);
        // expect_not_value(elosReceiveMessage, message, NULL);
        // will_return(elosReceiveMessage, result);
    }
}
