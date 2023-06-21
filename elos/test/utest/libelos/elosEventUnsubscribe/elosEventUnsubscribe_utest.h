// SPDX-License-Identifier: MIT
#ifndef __ELOS_ELOSEVENTUNSUBSCRIBE_H__
#define __ELOS_ELOSEVENTUNSUBSCRIBE_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_safu.h>

#include "mock_libelos_communication.h"

#define _MOCK_FD            42
#define _MOCK_EVENTQUEUE_ID 42

typedef struct elosTestState {
    elosSession_t session;
    elosMessage_t *request;
    size_t requestSize;
    char const *requestStr;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventUnsubscribeErrCommunication)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventUnsubscribeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventUnsubscribeErrRequest)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventUnsubscribeSuccess)

void elosMockSendAndReceiveJsonMessage(elosTestState_t *test, char const *responseStr, safuResultE_t result);

#endif /* __ELOS_ELOSEVENTUNSUBSCRIBE_H__ */
