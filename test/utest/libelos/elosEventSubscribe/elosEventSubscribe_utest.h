// SPDX-License-Identifier: MIT
#pragma once

#include <arpa/inet.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <netinet/in.h>

#include "mock_libelos_communication.h"

typedef struct elosTestRequest {
    elosMessage_t *message;
    const char *payload;
    size_t payloadLength;
    size_t length;
} elosTestRequest_t;

typedef struct elosTestResponse {
    elosMessage_t *message;
    const char *payload;
    size_t payloadLength;
    size_t length;
} elosTestResponse_t;

typedef struct _TestState {
    elosTestRequest_t request;
    elosTestResponse_t response;
    elosSession_t session;
} _TestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeErrEmptyList)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeErrInvalidParameter)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeErrorResponseHasNoIdsField)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeExterrCommunicationFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeExterrJsonAddNewArrayFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeExterrJsonAddNewStringFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeExterrJsonGetUint32Failed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSubscribeExterrJsonObjectNewObjectFailed)

void elosEventSubscribeUtestCreateMessage(elosMessage_t **message, size_t *messageLength, uint8_t messageId,
                                          const char *payload, size_t payloadSize);

void elosMockSendAndReceiveJsonMessage(_TestState_t *test, safuResultE_t result);
