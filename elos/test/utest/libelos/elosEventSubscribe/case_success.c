// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeSuccessSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeSuccessTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState->request.message);
    free(testState->response.message);
    free(testState);
    return 0;
}

void elosTestElosEventSubscribeSuccess(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "Create a subscription message, send it, evaluate the response and return list of message queue ids");

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

    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, expectedSession->fd);
    expect_memory(__wrap_safuSendExactly, buf, request->message, request->length);
    expect_value(__wrap_safuSendExactly, len, request->length);
    will_return(__wrap_safuSendExactly, request->length);

    MOCK_FUNC_ALWAYS(safuRecvExactly);
    expect_value(__wrap_safuRecvExactly, fd, expectedSession->fd);
    expect_any(__wrap_safuRecvExactly, buf);
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));
    will_set_parameter(__wrap_safuRecvExactly, buf, response->message);
    will_return(__wrap_safuRecvExactly, sizeof(elosMessage_t));

    expect_value(__wrap_safuRecvExactly, fd, expectedSession->fd);
    expect_any(__wrap_safuRecvExactly, buf);
    expect_value(__wrap_safuRecvExactly, len, response->payloadLength);
    will_set_parameter(__wrap_safuRecvExactly, buf, response->payload);
    will_return(__wrap_safuRecvExactly, response->payloadLength);

    const char *filter[] = {
        "event.source.appName,'myApp',eq",
        "event.source.appName,'linux',eq",
        "event.source.appName,'openssh',eq",
    };
    elosEventQueueId_t ids[3] = {0};

    safuResultE_t result = elosEventSubscribe(expectedSession, filter, ARRAY_SIZE(filter), ids);

    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(ids[0], 1);
    assert_int_equal(ids[1], 2);
    assert_int_equal(ids[2], 3);
}
