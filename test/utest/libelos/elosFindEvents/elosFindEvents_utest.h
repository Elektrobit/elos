// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>
#include <json-c/json_types.h>

#include "elos/event/event_types.h"
#include "elos/libelos/libelos.h"

#define VALID_SESSION_FD   42
#define INVALID_SESSION_FD (-1)

typedef struct {
    elosSession_t *session;
    size_t requestCount;  // incremented by mock
    json_object **expectedJsonRequest;
    json_object **expectedJsonResponse;
    elosEventVector_t *eventVector;
} elosUteststateT_t;

void elosFindEventsUtestCreateSession(void **state);
void elosFindEventsUtestFreeSession(void **state);

void elosMockSendAndReceiveJsonMessage(elosUteststateT_t *test, size_t callCount, safuResultE_t result);
void elosCleanupMockSendAndReceiveJsonMessage(elosUteststateT_t *test);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsErrInvalidParameter)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrVerifySession)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrNewObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrAddNewString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrAddNewTimestamp)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrCommunication)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsExtErrVectorFromJsonArr)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosFindEventsSuccessTruncated)
