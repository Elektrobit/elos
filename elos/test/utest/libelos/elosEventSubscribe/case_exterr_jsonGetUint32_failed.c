// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeExterrJsonGetUint32FailedSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeExterrJsonGetUint32FailedTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState->request.message);
    free(testState);
    return 0;
}

void elosTestElosEventSubscribeExterrJsonGetUint32Failed(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "Create a subscription message and check behavior if safuJsonGetUint32 fails.");

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

    response->payload = "{\"eventQueueIds\":[1,2,3]}";
    response->payloadLength = strlen(response->payload);
    elosEventSubscribeUtestCreateMessage(&response->message, &response->length, ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE,
                                         response->payload, response->payloadLength);

    elosMockSendAndReceiveJsonMessage(testState, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(safuJsonGetUint32, 0);
    expect_any(__wrap_safuJsonGetUint32, jobj);
    expect_value(__wrap_safuJsonGetUint32, name, NULL);
    expect_value(__wrap_safuJsonGetUint32, idx, 0);
    expect_any(__wrap_safuJsonGetUint32, val);
    will_set_parameter(__wrap_safuJsonGetUint32, val, 0);
    will_return(__wrap_safuJsonGetUint32, -1);

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
