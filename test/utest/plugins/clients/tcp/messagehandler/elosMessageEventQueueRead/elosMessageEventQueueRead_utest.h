// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGE_EVENTQUEUE_READ_H__
#define __ELOS_MESSAGE_EVENTQUEUE_READ_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/common/message.h"
#include "connectionmanager/connectionmanager.h"
#include "mock_message_handler.h"

#define MOCK_MESSAGE_MEMORY_SIZE 4096
#define MOCK_EVENTQUEUE_ID       1
#define _ERRSTR(__errstr)        "{\"error\":\"" __errstr "\"}"

typedef struct elosUnitTestState {
    elosClientConnection_t connection;
    elosClientConnectionSharedData_t sharedData;
    elosEventQueueId_t eventQueueId;
    elosEventVector_t *eventVector;
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
    elosMessage_t *message;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventQueueReadErrRequest)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventQueueReadErrQueueRead)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventQueueReadErrResponseCreate)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventQueueReadErrResponseSend)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventQueueReadSuccess)

void elosMessageEventQueueReadUtestSetupEventProcessor(void **state);
void elosMessageEventQueueReadUtestSetupMessage(void **state, char const *message);
extern safuResultE_t elosMessageEventQueueRead(elosClientConnection_t *conn, elosMessage_t const *const msg);

#endif /* __ELOS_MESSAGE_EVENTQUEUE_READ_H__ */
