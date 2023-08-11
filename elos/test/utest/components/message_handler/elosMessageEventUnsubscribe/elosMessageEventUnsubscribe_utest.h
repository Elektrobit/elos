// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEEVENTUNSUBSCRIBE_H__
#define __ELOS_MESSAGEEVENTUNSUBSCRIBE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/clientmanager/clientmanager.h"
#include "elos/common/message.h"
#include "mock_eventfiltermanager.h"
#include "mock_message_handler.h"

#define MOCK_MESSAGE_MEMORY_SIZE 4096
#define MOCK_EVENTQUEUE_ID       1
#define _ERRSTR(__errstr)        "{\"error\":\"" __errstr "\"}"

typedef struct elosUnitTestState {
    elosClientManagerConnection_t connection;
    elosClientManagerSharedData_t sharedData;
    elosEventQueueId_t eventQueueId;
    elosEventVector_t *eventVector;
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
    elosMessage_t *message;
} elosUnitTestState_t;

extern safuResultE_t elosMessageEventUnsubscribe(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

void elosMessageEventUnsubscribeUtestSetupEventProcessor(void **state);
void elosMessageEventUnsubscribeUtestSetupMessage(void **state, char const *message);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeErrFilterDelete)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeErrQueueDelete)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeErrRequest)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeErrResponseCreate)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeErrResponseSend)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventUnsubscribeSuccess)

#endif /* __ELOS_MESSAGEEVENTUNSUBSCRIBE_H__ */
