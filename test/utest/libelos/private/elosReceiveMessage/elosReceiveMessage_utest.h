// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>

#include "mock_libelos_communication.h"

typedef struct elosReceiveValues {
    int fd;
    void *buf;
    size_t len;
    size_t result;
} elosReceiveValues_t;

typedef struct elosReceiveSet {
    elosReceiveValues_t header;
    elosReceiveValues_t body;
    elosMessage_t *message;
    size_t messageSize;
} elosReceiveSet_t;

typedef struct elosUnitTestState {
    elosReceiveSet_t receiveJson;
    elosReceiveSet_t receiveNoJson;
    elosSession_t session;
    elosMessage_t *message;
} elosUnitTestState_t;

void elosMockReceiveExactlySetup(elosReceiveValues_t const *header, elosReceiveValues_t const *body);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveMessageErrAllocation)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveMessageErrReceiveBody)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveMessageErrReceiveHeader)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosReceiveMessageSuccess)
