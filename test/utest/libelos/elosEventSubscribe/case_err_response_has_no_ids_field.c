// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeErrorResponseHasNoIdsFieldSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeErrorResponseHasNoIdsFieldTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState->request.message);
    free(testState);
    return 0;
}

void elosTestElosEventSubscribeErrorResponseHasNoIdsField(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "Create a subscription message and check behavior if response has no 'eventQueueIds' field.");

    struct _TestState *testState = *state;
    elosSession_t *expectedSession = &testState->session;
    elosTestRequest_t *request = &testState->request;
    elosTestResponse_t *response = &testState->response;

    request->payload =
        "{\"filter\":["
        "\"event.source.appName,'myApp',eq\","
        "\"event.source.appName,'linux',eq\","
        "\"event.source.appName,'openssh',eq\""
        "]}";
    request->payloadLength = strlen(request->payload);
    elosEventSubscribeUtestCreateMessage(&request->message, &request->length, ELOS_MESSAGE_EVENT_SUBSCRIBE,
                                         request->payload, request->payloadLength);

    response->payload = "{}";
    response->payloadLength = strlen(response->payload);
    elosEventSubscribeUtestCreateMessage(&response->message, &response->length, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE,
                                         response->payload, response->payloadLength);

    elosMockSendAndReceiveJsonMessage(testState, SAFU_RESULT_OK);

    const char *filter[] = {
        "event.source.appName,'myApp',eq",
        "event.source.appName,'linux',eq",
        "event.source.appName,'openssh',eq",
    };
    elosEventQueueId_t id = ELOS_ID_INVALID;

    safuResultE_t result = elosEventSubscribe(expectedSession, filter, ARRAY_SIZE(filter), &id);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(id, ELOS_ID_INVALID);
}
