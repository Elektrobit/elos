// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>
#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_safu.h>

#include "mock_libelos_communication.h"

typedef struct elosUnitTestSet {
    elosEvent_t event;
    char *eventStr;
    elosMessage_t *message;
    size_t messageSize;
} elosUnitTestSet_t;

typedef struct elosUnitTestState {
    elosUnitTestSet_t normal;
    elosSession_t session;
} elosUnitTestState_t;

void elosMockSendAndReceiveJsonMessage(elosUnitTestState_t *test, elosUnitTestSet_t *tSet, safuResultE_t result);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventPublishErrNewObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventPublishErrConvert)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventPublishErrCommunication)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventPublishErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventPublishSuccess)
