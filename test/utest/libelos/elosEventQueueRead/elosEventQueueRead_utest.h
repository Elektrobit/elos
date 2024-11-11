// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_safu.h>

#include "mock_event.h"
#include "mock_libelos_communication.h"

#define _MOCK_FD            42
#define _MOCK_EVENTQUEUE_ID 42

typedef struct elosUnitTestState {
    elosSession_t session;
    elosEventVector_t *eventVector;
    elosMessage_t *request;
    size_t requestSize;
    char const *requestStr;
    char const *responseStrNormal;
    char const *responseStrEmpty;
    char const *responseStrNoArray;
} elosUnitTestState_t;

void elosMockSendAndReceiveJsonMessage(elosUnitTestState_t *test, char const *responseStr, safuResultE_t result);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventQueueReadErrConversion)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventQueueReadErrExchange)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventQueueReadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventQueueReadErrRequest)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventQueueReadSuccess)
