// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEHANDLERHANDLEMESSAGE_H__
#define __ELOS_MESSAGEHANDLERHANDLEMESSAGE_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/common/message.h>

#include "messagehandler/message_handler.h"
#include "safu/mock_safu.h"

typedef struct {
    elosClientConnection_t *connection;
    elosMessage_t *message;
    size_t messagePayloadLen;
} elosUteststateT_t;

void elosMessageHandlerHandleMessageUtestCreateConnection(void **state);
void elosMessageHandlerHandleMessageUtestFreeConnection(void **state);
void elosMessageHandlerHandleMessageUtestCreateMessage(void **state);
void elosMessageHandlerHandleMessageUtestDeleteMessage(void **state);

extern int elosMessageGetVersion(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrConnection)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageExterrFirstAllocMem)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrReadHeaderFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageExterrSecondAllocMem)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrReadPayloadFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrInvalidMessageId)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrMessageFunctionReturnFailure)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageHandlerHandleMessageErrLengthZero)

#define _RECV_MOCK(__testState)                                                                \
    MOCK_FUNC_ALWAYS(safuRecvExactly);                                                         \
    expect_value(__wrap_safuRecvExactly, fd, testState->connection->fd);                       \
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);                                       \
    expect_value(__wrap_safuRecvExactly, len, sizeof(elosMessage_t));                          \
    will_set_parameter(__wrap_safuRecvExactly, buf, (__testState)->message);                   \
    will_set_parameter(__wrap_safuRecvExactly, transferred, sizeof(elosMessage_t));            \
    will_return(__wrap_safuRecvExactly, SAFU_RESULT_OK);                                       \
                                                                                               \
    expect_value(__wrap_safuRecvExactly, fd, (__testState)->connection->fd);                   \
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);                                       \
    expect_value(__wrap_safuRecvExactly, len, (__testState)->messagePayloadLen);               \
    will_set_parameter(__wrap_safuRecvExactly, buf, (__testState)->message->json);             \
    will_set_parameter(__wrap_safuRecvExactly, transferred, (__testState)->messagePayloadLen); \
    will_return(__wrap_safuRecvExactly, SAFU_RESULT_OK);

#define _RECV_MOCK_CLEANUP MOCK_FUNC_NEVER(safuRecvExactly);

#endif
