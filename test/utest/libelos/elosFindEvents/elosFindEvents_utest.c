// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "elosFindEvents_utest.h"

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <json-c/json_object.h>
#include <stdio.h>

#include "mock_libelos_communication.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosFindEventsErrInvalidParameter),     TEST_CASE(elosTestElosFindEventsExtErrVerifySession),
        TEST_CASE(elosTestElosFindEventsExtErrNewObject),         TEST_CASE(elosTestElosFindEventsExtErrAddNewString),
        TEST_CASE(elosTestElosFindEventsExtErrAddNewTimestamp),   TEST_CASE(elosTestElosFindEventsExtErrCommunication),
        TEST_CASE(elosTestElosFindEventsExtErrVectorFromJsonArr), TEST_CASE(elosTestElosFindEventsSuccess),
        TEST_CASE(elosTestElosFindEventsSuccessTruncated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

void elosFindEventsUtestCreateSession(void **state) {
    elosUteststateT_t *testState = *state;
    testState->session = safuAllocMem(NULL, sizeof(elosSession_t));
    testState->session->fd = VALID_SESSION_FD;
    testState->session->connected = true;
    assert_non_null(testState->session);
}

void elosFindEventsUtestFreeSession(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->session);
}

int _verifyExpectedRequest(const LargestIntegralType request, const LargestIntegralType testState) {
    elosUteststateT_t *state = (elosUteststateT_t *)testState;
    json_object *actualJsonRequest = (json_object *)request;
    int isEqual = json_object_equal(actualJsonRequest, state->expectedJsonRequest[state->requestCount]);
    if (isEqual == 0) {
        print_error("Request is not as expected: actual '%s' !=  expected '%s'\n",
                    json_object_to_json_string(actualJsonRequest),
                    json_object_to_json_string(state->expectedJsonRequest[state->requestCount]));
    }
    state->requestCount++;
    return isEqual;
}

void elosMockSendAndReceiveJsonMessage(elosUteststateT_t *test, size_t callCount, safuResultE_t result) {
    MOCK_FUNC_ALWAYS(elosSendAndReceiveJsonMessage);
    for (size_t i = 0; i < callCount; i++) {
        expect_value(elosSendAndReceiveJsonMessage, session, test->session);
        expect_value(elosSendAndReceiveJsonMessage, messageId, ELOS_MESSAGE_LOG_FIND_EVENT);
        expect_check(elosSendAndReceiveJsonMessage, sendJsonObject, _verifyExpectedRequest, test);

        expect_not_value(elosSendAndReceiveJsonMessage, receiveJsonObject, NULL);
        will_set_parameter(elosSendAndReceiveJsonMessage, receiveJsonObject, test->expectedJsonResponse[i]);
        will_return(elosSendAndReceiveJsonMessage, result);
    }
}

void elosCleanupMockSendAndReceiveJsonMessage(elosUteststateT_t *test) {
    for (size_t i = 0; i < test->requestCount; i++) {
        if (test->expectedJsonRequest[i] != NULL) {
            json_object_put(test->expectedJsonRequest[i]);
        }
        // testState->expectedJsonResponse is freed by the implementation
    }
    free(test->expectedJsonResponse);
    free(test->expectedJsonRequest);
    MOCK_FUNC_NEVER(elosSendAndReceiveJsonMessage);
}
