// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/common/message.h"
#include "mock_libelos_communication.h"

#define VALID_SESSION_FD   42
#define INVALID_SESSION_FD (-1)
#define MOCK_RESPONSE_JOBJ "{\"eventArray\": [{\"messageCode\":1}, {\"payload\":\"" MOCK_PAYLOAD "\"} ]}"
#define MOCK_MESSAGECODE   1
#define MOCK_PAYLOAD       "123"

typedef struct {
    elosSession_t *session;
    elosMessage_t *request;
    elosMessage_t *response;
    char *responsePayload;
    size_t requestLength;
    size_t responseLength;
    size_t responsePayloadLength;
} elosUteststateT_t;

void elosLogFindEventUtestCreateSession(void **state);
void elosLogFindEventUtestFreeSession(void **state);
void elosLogFindEventUtestCreateMessage(elosMessage_t **message, size_t *messageLength, uint8_t messageId,
                                        const char *payload, size_t payloadSize);
void elosLogFindEventUtestDeleteMessage(elosMessage_t *message);
void elosLogFindEventUtestCreateResponse(void **state);
void elosLogFindEventUtestDeleteResponse(void **state);

void elosMockSendAndReceiveJsonMessage(elosUteststateT_t *test, char const *responseStr, safuResultE_t result);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventErrSession)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventErrFilterRule)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventErrVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventExtErrVerifySession)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventExtErrNewObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventExtErrAddNewString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventExtErrCommunication)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventExtErrVectorFromJsonArr)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogFindEventSuccess)

#define _SEND_RECV_MOCK(__testState)                                                                  \
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);                                                         \
    expect_value(__wrap_safuSendExactly, fd, VALID_SESSION_FD);                                       \
    expect_memory(__wrap_safuSendExactly, buf, (__testState)->request, (__testState)->requestLength); \
    expect_value(__wrap_safuSendExactly, len, (__testState)->requestLength);                          \
    will_return(__wrap_safuSendExactly, (__testState)->requestLength);                                \
                                                                                                      \
    MOCK_FUNC_ALWAYS(safuRecvExactly);                                                                \
    expect_value(__wrap_safuRecvExactly, fd, VALID_SESSION_FD);                                       \
    expect_any(__wrap_safuRecvExactly, buf);                                                          \
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));                                 \
    will_set_parameter(__wrap_safuRecvExactly, buf, (__testState)->response);                         \
    will_return(__wrap_safuRecvExactly, sizeof(elosMessage_t));                                       \
                                                                                                      \
    expect_value(__wrap_safuRecvExactly, fd, VALID_SESSION_FD);                                       \
    expect_any(__wrap_safuRecvExactly, buf);                                                          \
    expect_value(__wrap_safuRecvExactly, len, (__testState)->responsePayloadLength);                  \
    will_set_parameter(__wrap_safuRecvExactly, buf, (__testState)->responsePayload);                  \
    will_return(__wrap_safuRecvExactly, (__testState)->responsePayloadLength);

#define _SEND_RECV_MOCK_CLEANUP MOCK_FUNC_NEVER(safuRecvExactly);
